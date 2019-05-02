#include "mem.h"
#include "bits.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

mem::mem(string filename, string memorydump) {
  if (memorydump != "") {
    dumpmemory = true;
    memorydumpfile = memorydump;
  }
  load_cart(filename);
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
  
  lcdmode = 1;
  memory[O_IO + IO_LCDSTAT] = 0x91;

  divtimer = 0;
  timatimer = 0;
  tacthreshold = tacvals[0];
  dmatimer = 0;
  dmatransfering = false;
  dmasrc = 0x00;
  //0x0f is no keys pressed
  joydirection = 0x0f;
  joyspecial = 0x0f;
  //initiallize joypad register to something sensible
  loadeddirection = true;
  //direction is actually selected since setting the bit disables it
  memory[O_IO + IO_JOYP] = (JOYP_SPECIAL_SELECTED|0x0f);
  cout << "memory initialized\n";
}
void mem::write_byte_internal(uint16 address, uint8 data) {
  //this can only take values between 0 and 153 but step_lcd takes care of that
  if (address == O_IO+IO_LY) {
    assert(data < 154);
    memory[O_IO+IO_LY] = data;
  }
  else if (address == O_IO+IO_LCDSTAT) {
    memory[O_IO+IO_LCDSTAT] = data;
    lcdmode = data & 0x03;
  }
  else {
    memory[address] = data;
  }
}
void mem::write_byte(uint16 address, uint8 data) {
  if (dmatransfering) {
    if ((address >= O_HRAM) && (address != O_IE)) {
      memory[address] = data;
    }
    return;
  }
  //else if (((address < O_UNUSED) && (address >= O_OAM)) && ((lcdmode == 2) || (lcdmode == 3))) {
  //  //can't access OAM during lcd modes 2 and 3
  //  return;
  //}
  //else if (((address < O_ERAM) && (address >= O_VRAM)) && (lcdmode == 3)) {
  //  //can't access VRAM during lcd mode 3
  //  return;
  //}
  else if (address == O_IO+IO_LCDSTAT) {
    //lower 3 bits of LCD STAT are read only
    //also since we can't change lcd mode directly, lcdmode doesn't have to be updated here
    memory[O_IO+IO_LCDSTAT] = (memory[O_IO+IO_LCDSTAT] & 0x07) | (data & 0xf8);
    return;
  }
  else if (address == O_IO+IO_LY) {
    //this is read only
    return;
  }
  else if (address == O_IO+IO_BGP) {
    memory[address] = data;
    update_palette(2, data);
    return;
  }
  else if (address == O_IO+IO_OBP0) {
    memory[address] = data;
    update_palette(0, data);
    return;
  }
  else if (address == O_IO+IO_OBP1) {
    memory[address] = data;
    update_palette(1, data);
    return;
  }
  else if (address == O_IO+IO_LY) {
    //this is read only, attempting to write to it "resets the counter"
    memory[O_IO+IO_LY] = 0;
    return;
  }
  else if (address == O_IO+IO_DIV) {
    memory[O_IO+IO_DIV] = 0x00;
    return;
  }
  else if (address == O_IO+IO_TAC) {
    tacthreshold = tacvals[data & 0x03];
    memory[O_IO+IO_TAC] = data;
    return;
  }
  else if (address == O_IO+IO_JOYP) {
    uint8 keysselected = data & 0x30;
    //if special keys are selected
    if (keysselected == JOYP_DIRECTION_SELECTED) {
      loadeddirection = false;
      memory[O_IO+IO_JOYP] = (joyspecial & 0x0f)|(data & 0xf0);
      return;
    }
    //if direction keys are selected
    else if (keysselected == JOYP_SPECIAL_SELECTED) {
      loadeddirection = true;
      memory[O_IO+IO_JOYP] = (joydirection & 0x0f)|(data & 0xf0);
      return;
    }
    //if neither set is selected
    else if (keysselected == (JOYP_DIRECTION_SELECTED|JOYP_SPECIAL_SELECTED)) {
      //keep previous setting for loadeddirection
      memory[O_IO+IO_JOYP] = (data & 0xf0);
      return;
    }
    //if both sets are selected (undefined behavior)
    else if (keysselected == 0x00) {
      //ignore attempt to write to memory
      return;
    }
  }
  else if (address == O_IO+IO_DMA) {
    if (data <= 0xF1) {
      dmatimer = 160;
      dmatransfering = true;
      dmasrc = data;
      //for (int i = 0; i < 160; i++) {
      //  memory[0xFE00 + i] = memory[(data << 8) + i];
      //}
    }
  }
  else if (address < 0x8000) {
    cout << hex <<(int)address << " " << hex << (int)data<< " tried writing to ROM\n";
    return;
  }
  else {
    memory[address] = data;
  }
};

void mem::load_cart(string filename)
{
  streampos size;
  ifstream cart;
  //open file with pointer positioned at the end
  cart.open(filename, ios::binary|ios::ate);
  assert(cart.is_open());
  size = cart.tellg();
  //rombn.resize((int)size - (int)O_ROMBN);
  cart.seekg(0,ios::beg);
  cart.read((char *) &memory[0],0x8000);
  //cart.read((char *) &romb0[0],O_ROMBN);
  //cart.read((char *) &rombn[0],(int)size-(int)O_ROMBN);
  cart.close();
  for (int i =0x0134; i < 0x0144; i++) cout << read_byte(i);//print title
  cout << (read_byte(0x014A) ? "\nNon-Japanese" : "\nJapanese");
  cout << (read_byte(0x0147) == 0x00 ? "\ncartridge is ROM only" : "\ncartridge includes MBC (not implemented)");
  cout << "\n";
}
void mem::update_palette(uint8 palette, uint8 value)
{
  int j = 0;
  for (int i = 0x03; i < 0xff; i = i << 2)
  {
    switch ((value & i) >> (2*j))
    {
      case 0: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0xcc,0xcc,0xcc};break;}
      case 1: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0x88,0x88,0x88};break;}
      case 2: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0x66,0x66,0x66};break;}
      case 3: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0x33,0x33,0x33};break;}
    }
    j++;
  }
}

array<color,4> mem::get_palette(uint8 palette_num)
{
  return palettes[palette_num];
}
void mem::dump_memory()
{
  ofstream dump;
  dump.open(memorydumpfile);
  assert(dump.is_open());
  for (uint16 i = 0; i < 0xffff; i++) {
    if (read_byte(i) != 0) {
      dump << "[0x" << hex << i << "]  0x" << hex << (int)read_byte(i) << endl;
    }
  }
  dump.close();
}
void mem::update_timers(int dt) {
  //compute increment based on time opcode takes to give a 16.384 kHz increment rate
  //increment every 256 CPU clicks
  //this should be independent of CPU throttling which is artificial
  divtimer += dt;
  if (divtimer > 64) {
    divtimer -= 64;
    write_byte_internal(O_IO+IO_DIV, read_byte(O_IO+IO_DIV)+1);
  }
  if (read_byte(O_IO+IO_TAC) & TIMER_ENABLED) {
    timatimer += dt;
    if (timatimer > tacthreshold) {
      timatimer -= tacthreshold;
      write_byte_internal(O_IO+IO_TIMA, read_byte(O_IO+IO_TIMA)+1);
      //if overflow load value in TMA register
      if (read_byte(O_IO+IO_TIMA) == 0x00) {
        write_byte_internal(O_IO+IO_TIMA, read_byte(O_IO+IO_TMA));
        //request a timer interrupt
        write_byte_internal(O_IO+IO_IR, read_byte(O_IO+IO_IR) | INT_TIM);
      }
    }
  }
  if (dmatimer != 0) {
    for (int i = 160-dmatimer; i < MIN(160-dmatimer+dt,160); i++) {
      memory[0xFE00 + i] = memory[(dmasrc << 8) + i];
    }
    dmatimer -= dt;
    if (dmatimer <= 0) {
      dmatimer = 0;
      dmatransfering = false;
    }
  }
}

void mem::update_keys(bool special, uint8 bit, bool down) {
  //cout << hex << (int)memory[O_IO+IO_JOYP] << " " << hex << (int)joyspecial << " " << hex << (int)joydirection << "  to  ";
  if (special) {
    if (down) {
      //if the key for a special button is pressed, clear that bit in joyspecial
      joyspecial &= ~bit;
    }
    else {
      //if the key for a special button is released, set that bit in joyspecial
      joyspecial |= bit;
    }
    //if joyspecial is currently loaded, update the lower 4 bits in memory
    if (!loadeddirection) {
      write_byte_internal(O_IO+IO_JOYP,(joyspecial & 0x0f) | (read_byte(O_IO+IO_JOYP) & 0xf0));
    }
  }
  else {
    if (down) {
      //if the key for a direction button is pressed, clear that bit in joydirection
      joydirection &= ~bit;
    }
    else {
      //if the key for a direction button is released, set that bit in joydirection
      joydirection |= bit;
    }
    //if joydirection is currently loaded, update the lower 4 bits in memory
    if (loadeddirection) {
      write_byte_internal(O_IO+IO_JOYP,(joydirection & 0x0f) | (read_byte(O_IO+IO_JOYP) & 0xf0));
    }
  }
  //cout << hex << (int)memory[O_IO+IO_JOYP] << " " << hex << (int)joyspecial << " " << hex << (int)joydirection << endl;
}
uint8 mem::get_keys(bool special) {
  if (special) {
    return joyspecial;
  }
  else {
    return joydirection;
  }
}
bool mem::direction_loaded() {
  return loadeddirection;
}
