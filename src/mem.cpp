#include "mem.h"
#include "bits.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
  Constructor for \ref mem
*/
mem::mem(string filename, string memorydump, string savefile) {
  if (memorydump != "") {
    dumpmemory = true;
    memorydumpfile = memorydump;
  }
  load_cart(filename);
  print_cart_info();

  ramenabled = false;
  mbcmode = rom;
  current_rombank = 1;
  current_rambank = 0;
  if (savefile != "") {
    ramdumpfile = savefile;
  }
  else {
    int delimiter_position = filename.rfind("/");
    ramdumpfile = filename.substr(delimiter_position+1,filename.length()-1-delimiter_position);
    ramdumpfile = "saves/" + ramdumpfile.substr(0,ramdumpfile.length()-3) + ".sav";
  }
  load_ram();

  memory[O_IO + IO_TIMA] = 0x00;
  memory[O_IO + IO_TMA] = 0x00;
  memory[O_IO + IO_TAC] = 0x00;
  memory[O_IO + IO_NR10] = 0x80;
  memory[O_IO + IO_NR11] = 0xbf;
  memory[O_IO + IO_NR12] = 0xf3;
  memory[O_IO + IO_NR14] = 0xbf;
  memory[O_IO + IO_NR21] = 0x3f;
  memory[O_IO + IO_NR22] = 0x00;
  memory[O_IO + IO_NR24] = 0xbf;
  memory[O_IO + IO_NR30] = 0x7f;
  memory[O_IO + IO_NR31] = 0xff;
  memory[O_IO + IO_NR32] = 0x9f;
  memory[O_IO + IO_NR33] = 0xbf;
  memory[O_IO + IO_NR41] = 0xff;
  memory[O_IO + IO_NR42] = 0x00;
  memory[O_IO + IO_NR43] = 0x00;
  memory[O_IO + IO_NR44] = 0xbf;
  memory[O_IO + IO_NR50] = 0x77;
  memory[O_IO + IO_NR51] = 0xf3;
  memory[O_IO + IO_NR52] = 0xf1;
  memory[O_IO + IO_LCDC] = 0x91;
  memory[O_IO + IO_SCY] = 0x00;
  memory[O_IO + IO_SCX] = 0x00;
  memory[O_IO + IO_LYC] = 0x00;
  memory[O_IO + IO_BGP] = 0xfc;
  memory[O_IO + IO_OBP0] = 0xff;
  memory[O_IO + IO_OBP1] = 0xff;
  memory[O_IO + IO_WY] = 0x00;
  memory[O_IO + IO_WX] = 0x00;
  memory[O_IE] = 0x00;
  
  memory[O_IO + IO_LCDSTAT] = 0x91;
  update_palette(2, 0xfc);
  update_palette(0, 0xff);
  update_palette(1, 0xff);

  divtimer = 0;
  timatimer = 0;
  tacthreshold = tacvals[0];
  dmatimer = 0;
  dmatransfering = false;
  rtc_timer = 0;
  ch1timer = 0;
  ch2timer = 0;
  ch3timer = 0;
  ch4timer = 0;
  for (auto it = rtc_registers.begin(); it != rtc_registers.end(); it++) {
    *it = 0;
  }
  for (auto it = latched_rtc_registers.begin(); it != latched_rtc_registers.end(); it++) {
    *it = 0;
  }
  //0x0f is no keys pressed
  joydirection = 0x0f;
  joyspecial = 0x0f;
  //initiallize joypad register to something sensible
  loadeddirection = true;
  //direction is actually selected since setting the bit disables it
  memory[O_IO + IO_JOYP] = (JOYP_SPECIAL_SELECTED|0x0f);
  wait.tv_sec = 0;
  wait.tv_nsec = 1;
}

/**
  Read a byte from memory when the address is guaranteed to be outside the ROM/RAM bank address space
*/
uint8 mem::read_byte_internal(uint16 address) {
  return memory[address];
}

/**
  Read a byte from memory at an arbitrary address
*/
uint8 mem::read_byte(uint16 address) {
  if ((address < O_VRAM) && (address >= 0x4000)) {
    return *(rombank_ptr + address - O_ROMBN);
  }
  else if ((address < O_WRAM0) && (address >= O_ERAM)) {
    if (mbcmode == rtc) {
      return latched_rtc_registers[current_rtc];
    }
    else {
      return *(rambank_ptr + address - O_ERAM);
    }
  }
  else {
    return memory[address];
  }
}

/**
  Read a word from memory when the address is guaranteed to be outside the ROM/RAM bank address space
*/
uint16 mem::read_word_internal(uint16 address) {
  return (read_byte_internal(address))+(read_byte_internal(address + 1) << 8);
}

/**
  Read a word from memory at an arbitrary address
*/
uint16 mem::read_word(uint16 address) {
  uint16 data = (read_byte(address))+(read_byte(address + 1) << 8);
  cout << "read " << hex << (int)data << " from " << hex << (int)address << endl;
  return data;
}

/**
  Write a byte to memory without side effects
*/
void mem::write_byte_internal(uint16 address, uint8 data) {
  memory[address] = data;
}
 
/**
  Write a byte to memory taking into account write permissions and execute side effects
*/
void mem::write_byte(uint16 address, uint8 data) {
  //writing to anything but HRAM is restricted during DMA transfer
  if (dmatransfering) {
    if ((address >= O_HRAM) && (address != O_IE)) {
      memory[address] = data;
    }
    return;
  }
  //writing to 0x0000 - 0x7fff and 0xa000 - 0xbfff
  else if ((address < O_VRAM) || ((address >= O_ERAM) && (address < O_WRAM0))) {
    (*this.*handle_mbc)(address, data);
    return;
  }
  //writing to 0x8000 - 0x9fff
  else if ((address < O_ERAM) && (address >= O_VRAM)) {
    //can't access VRAM during lcd mode 3
    //if ((memory[O_IO + IO_LCDSTAT] & 0x03) != 3) {
      memory[address] = data;
    //}
    return;
  }
  //writing to 0xc000 - 0xdfff
  else if ((address >= O_WRAM0) && (address < O_ECHO0)) {
    memory[address] = data;
    if ((address - O_WRAM0) < 0xe00) {
      memory[address + 0x2000] = data;
    }
  }
  //writing to 0xe000 - 0x0xfdff
  else if ((address >= O_ECHO0) && (address < O_OAM)) {
    memory[address] = data;
    memory[address - 0x2000] = data;
  }
  //writing to 0xfe00 - 0xfea0
  else if ((address < O_UNUSED) && (address >= O_OAM)) {
    //can't access OAM during lcd modes 2 and 3
    if (((memory[O_IO + IO_LCDSTAT] & 0x03) != 2) && ((memory[O_IO + IO_LCDSTAT] & 0x03) != 3)) {
      memory[address] = data;
    }
    return;
  }
  //writing to 0xfea0 - 0xfeff
  else if ((address >= O_UNUSED) && (address < O_IO)) {
    return;
  }
  //writing to 0xff00 - 0xff7f
  else if ((address >= O_IO) && (address < O_HRAM)) {
    switch(address - O_IO) {
      #include "io.h"
    }
    return;
  }
  //writing to 0xff80 - 0xffff
  else {
    memory[address] = data;
    return;
  }
}

/**
  Write a word to memory taking into account write permissions and execute side effects
*/
void mem::write_word(uint16 address, uint16 data) {
  cout << "writing " << hex << (int)data << " to " << hex << (int)address << endl;
  write_byte(address, data & 0x00ff);
  write_byte(address+1, data >> 8);
}

/**
  Get one of the palettes stored in \ref mem
*/
array<color,4> mem::get_palette(uint8 palette_num) {
  return palettes[palette_num];
}

void mem::update_palette(uint8 palette, uint8 value) {
  int j = 0;
  for (int i = 0x03; i < 0xff; i = i << 2) {
    switch ((value & i) >> (2*j)) {
      case 0: {palettes[palette][j] = {0xcc,0xcc,0xcc,SDL_ALPHA_TRANSPARENT};break;}
      case 1: {palettes[palette][j] = {0x88,0x88,0x88,SDL_ALPHA_OPAQUE};break;}
      case 2: {palettes[palette][j] = {0x66,0x66,0x66,SDL_ALPHA_OPAQUE};break;}
      case 3: {palettes[palette][j] = {0x33,0x33,0x33,SDL_ALPHA_OPAQUE};break;}
    }
    j++;
  }
}

/**
  Dump addressable contents of memory to a file. This function only dumps the ROM/RAM banks that are loaded when it's called.
*/
void mem::dump_memory() {
  ofstream dump;
  dump.open(memorydumpfile);
  for (uint16 i = 0; i < 0xffff; i++) {
    dump << read_byte(i);
  }
  dump.close();
}

/**
  Get dumpmemory which is set if an output file was specified for \ref dump_memory()
*/
bool mem::get_dumpmemory() {
  return dumpmemory;
}

/**
  Dump contents of all RAM banks to a file. This function ensures that the originally loaded RAM bank is reloaded at the end so it may be called at any time.
*/
void mem::dump_ram() {
  ofstream dump;
  dump.open(ramdumpfile);
  int original_rambank = current_rambank;
  //for each ram bank
  for (int i = 0; i < num_rombanks; i++) {
    switch_rambanks(i);
    //iterate through ram bank addresses
    for (int j = O_ERAM; j < O_WRAM0; j++) {
      //save each byte to file
      dump << read_byte(j);
    }
  }
  switch_rambanks(original_rambank);
  dump.close();
}

/**
  Loads contents of a file into the RAM banks.
*/
void mem::load_ram() {
  ifstream readram;
  readram.open(ramdumpfile, ios::binary);
  if (readram.good()) {
    for (int i = 0; i <num_rombanks; i++) {
      switch_rambanks(i);
      readram.read((char *) rambank_ptr,0x2000);
    }
  }
}

/**
  Update the internal representation of the joypad register and trigger an interrupt if necessary
*/
void mem::update_keys(keyset k, uint8 bit, keystate kp) {
  if (k == special) {
    if (kp == press) {
      //if the key for a special button is pressed, clear that bit in joyspecial
      joyspecial &= ~bit;
    }
    else { //kp == release
      //if the key for a special button is released, set that bit in joyspecial
      joyspecial |= bit;
    }
    //if joyspecial is currently loaded, update the lower 4 bits in memory
    if (!loadeddirection) {
      write_byte_internal(O_IO+IO_JOYP,(joyspecial & 0x0f) | (read_byte_internal(O_IO+IO_JOYP) & 0xf0));
    }
  }
  else {
    if (kp == press) {
      //if the key for a direction button is pressed, clear that bit in joydirection
      joydirection &= ~bit;
    }
    else { //kp == release
      //if the key for a direction button is released, set that bit in joydirection
      joydirection |= bit;
    }
    //if joydirection is currently loaded, update the lower 4 bits in memory
    if (loadeddirection) {
      write_byte_internal(O_IO+IO_JOYP,(joydirection & 0x0f) | (read_byte_internal(O_IO+IO_JOYP) & 0xf0));
    }
  }
}

/**
  Returns the register representing the set of keys <code>k</code>
*/
uint8 mem::get_keys(keyset k) {
  return (k == special ? joyspecial : joydirection);
}

/**
  Returns the name of the currently loaded set of keys
*/
keyset mem::get_keys_loaded() {
  return (loadeddirection ? direction : special);
}

/**
  Increment internal timers and counters in memory. Also updates timers associated with generating sound and the MBC3 real-time clock registers.
*/
void mem::update_timers(int dt) {
  dt *= 4;
  //compute increment based on time opcode takes to give a 16.384 kHz increment rate
  //increment every 256 CPU clicks
  //this should be independent of CPU throttling which is artificial
  divtimer += dt;
  if (divtimer > 64) {
    divtimer -= 64;
    write_byte_internal(O_IO+IO_DIV, read_byte_internal(O_IO+IO_DIV)+1);
  }
  if (read_byte_internal(O_IO+IO_TAC) & TIMER_ENABLED) {
    timatimer += dt;
    if (timatimer > tacthreshold) {
      timatimer -= tacthreshold;
      write_byte_internal(O_IO+IO_TIMA, read_byte_internal(O_IO+IO_TIMA)+1);
      //if overflow load value in TMA register
      if (read_byte_internal(O_IO+IO_TIMA) == 0x00) {
        write_byte_internal(O_IO+IO_TIMA, read_byte_internal(O_IO+IO_TMA));
        //request a timer interrupt
        write_byte_internal(O_IO+IO_IR, read_byte_internal(O_IO+IO_IR) | INT_TIM);
      }
    }
  }
  if (dmatimer != 0) {
    dmatimer -= dt;
    if (dmatimer <= 0) {
      dmatimer = 0;
      dmatransfering = false;
    }
  }
  if (ch1timer > 0) {
    ch1timer -= dt;
    if (ch1timer <= 0) {
      ch1timer = 0;
      //if 6th bit is set stop output when length in NR11 expires
      if (read_byte_internal(O_IO + IO_NR14) & 0x40) {
        memory[O_IO + IO_NR52] &= 0x01;
      }
    }
  }
  if (ch2timer > 0) {
    ch2timer -= dt;
    if (ch2timer <= 0) {
      ch2timer = 0;
      //if 6th bit is set stop output when length in NR21 expires
      if (read_byte_internal(O_IO + IO_NR24) & 0x40) {
        memory[O_IO + IO_NR52] &= 0x02;
      }
    }
  }
  if (ch3timer > 0) {
    ch3timer -= dt;
    if (ch3timer <= 0) {
      ch3timer = 0;
      //if 6th bit is set stop output when length in NR31 expires
      if (read_byte_internal(O_IO + IO_NR34) & 0x40) {
        memory[O_IO + IO_NR52] &= 0x04;
      }
    }
  }
  if (ch4timer > 0) {
    ch4timer -= dt;
    if (ch4timer <= 0) {
      ch4timer = 0;
      //if 6th bit is set stop output when length in NR41 expires
      if (read_byte_internal(O_IO + IO_NR44) & 0x40) {
        memory[O_IO + IO_NR52] &= 0x08;
      }
    }
  }
  //update clock register in MBC3
  //I should probably use a function pointer to choose an update_timer() function based on the MBC instead of checking mbctype after each opcode is executed, but this is fine for prototyping
  if (mbctype == mbc3) {
    rtc_timer += dt;
    //check if one second has elapsed
    //threshold isn't exactly 1e6 since 1 cpu cycle isn't exactly 1 us
    if (rtc_timer >= 1048576) {
      //cout << hex << (int)rtc_registers[0] << ", " << hex << (int)latched_rtc_registers[0] << endl;
      rtc_timer -= 1048576;
      //rtc increment implemented recursively for simplicity
      increment_clock(&rtc_registers[0], &clock_maxvals[0]);
    }
  }
}

//using recursion to update clock for simplicity since this is cleaner than using 5 nested register updates
void mem::increment_clock(uint8 *reg, const uint8 *maxval) {
  *reg += 1;
  *reg %= *maxval;
  if ((*reg == 0) && (*maxval != 1)) {
    increment_clock(reg + 1, maxval + 1);
  }
}
