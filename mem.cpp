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
  //remember to set initial values for NR registers (sound)
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
  //initialized to 0x91 so lcd mode is initially VBLANK(0x01)
  //"" "" to 0x79 so "" "" and all other lcd interrupts are enabled
  //memory[O_IO + IO_LCDSTAT] = 0x93;
  memory[O_IO + IO_LCDSTAT] = 0x73;
  divtimer = 0;
  timatimer = 0;
  tacthreshold = tacvals[0];
  dmatimer = 0;
  //0x0f is no keys pressed
  joydirection = 0x0f;
  joyspecial = 0x0f;
  //initiallize joypad register to something sensible
  loadeddirection = true;
  //direction is actually selected since setting the bit disables it
  memory[O_IO + IO_JOYP] = (JOYP_SPECIAL_SELECTED|0x0f);
  cout << "memory initialized\n";
}
//!Adds offset to address if trying to modify ROM Bank N or External RAM
void mem::write_byte(uint16 address, uint8 data) {
  //prevent writing to OAM while LCD is writing to OAM
  //if ((address < O_UNUSED) && (address >= O_OAM) && (memory[O_IO+IO_LCDSTAT] & 0x02)) {
  //  return;
  //}
  ////prevent writing to OAM and VRAM while LCD is writing to both
  //else if ((((address < O_UNUSED) && (address >= O_OAM)) || ((address < O_ERAM) && (address >= O_VRAM))) && (memory[O_IO+IO_LCDSTAT] & 0x03)) {
  //  return;
  //}
  if(dmatimer != 0) {
    if ((address >= O_HRAM) && (address != O_IE)) {
      memory[address] = data;
    }
  }
  else {
  if (address != O_IO+IO_JOYP) {memory[address] = data;}
  //not totally sure if modifying IO_IR and IO_LCDSTAT is necessary after updating the palettes
  if (address == O_IO+IO_BGP) {
    update_palette(2,memory[O_IO + IO_BGP]);
    //memory[O_IO + IO_IR] &= 0x1f;
    //memory[O_IO + IO_LCDSTAT] &= 0x78;
  }
  else if (address == O_IO+IO_OBP0) {
    update_palette(0,memory[O_IO + IO_OBP0]);
    //memory[O_IO + IO_IR] &= 0x1f;
    //memory[O_IO + IO_LCDSTAT] &= 0x78;
  }
  else if (address == O_IO+IO_OBP1) {
    update_palette(1,memory[O_IO + IO_OBP1]);
    //memory[O_IO + IO_IR] &= 0x1f;
    //memory[O_IO + IO_LCDSTAT] &= 0x78;
  }
  else if (address == O_IO+IO_LY) {
    //memory[O_IO+IO_LY] = 0x00;
    //not sure what "reset the counter" means
  }
  else if (address == O_IO+IO_DIV) {
    memory[O_IO+IO_DIV] = 0x00;
  }
  else if (address == O_IO+IO_TAC) {
    tacthreshold = tacvals[data & 0x03];
  }
  else if (address == O_IO+IO_JOYP) {
    uint8 keysselected = data & 0x30;
    //if special keys are selected
    if (keysselected == JOYP_DIRECTION_SELECTED) {
      //cout << hex << (int)data << " game tried accessing special keys!\n";
      //toggle internal memory flag to false
      loadeddirection = false;
      //load saved joydirection into memory
      //also load top 4 bits of data
      memory[O_IO+IO_JOYP] = (joyspecial & 0x0f)|(data & 0xf0);
    }
    //if direction keys are selected
    else if (keysselected == JOYP_SPECIAL_SELECTED) {
      //cout << hex << (int)data << " game tried accessing direction keys!\n";
      //toggle internal memory flag to true
      loadeddirection = true;
      //load saved joydirection into memory
      //also load top 4 bits of data
      memory[O_IO+IO_JOYP] = (joydirection & 0x0f)|(data & 0xf0);
    }
    else if (keysselected == (JOYP_SPECIAL_SELECTED|JOYP_DIRECTION_SELECTED)) {
      //keep same set of keys loaded
      memory[O_IO+IO_JOYP] |= (JOYP_SPECIAL_SELECTED|JOYP_DIRECTION_SELECTED);
      //cout << hex << (int)data << " game tried blocking both sets of keys!\n";
    }
    else if (keysselected == 0x00) {
      //cout << hex << (int)data << " game tried accessing both sets of keys!\n";
      memory[O_IO+IO_JOYP] = ((data & 0xf0)|0x0f);
    }
  }
  else if (address == O_IO+IO_DMA) {
    dmatimer = 160;
    if (data <= 0xF1) {
      for (int i = 0; i < 160; i++) {
        memory[0xFE00 + i] = memory[(data << 8) + i];
      }
    }
  }
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
  for (int i =0x0134; i < 0x0144; i++) cout << memory[i];//print title
  cout << (memory[0x014A] ? "\nNon-Japanese" : "\nJapanese");
  cout << (memory[0x0147] == 0x00 ? "\ncartridge is ROM only" : "\ncartridge includes MBC (not implemented)");
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
    //directly accessing memory since writing to it resets it to zero
    memory[O_IO+IO_DIV]++;
  }
  if (memory[O_IO+IO_TAC] & TIMER_ENABLED) {
    timatimer += dt;
    if (timatimer > tacthreshold) {
      timatimer -= tacthreshold;
      memory[O_IO+IO_TIMA]++;
      //if overflow load value in TMA register
      if (memory[O_IO+IO_TIMA] == 0x00) {
        memory[O_IO+IO_TIMA] = memory[O_IO+IO_TMA];
        //request a timer interrupt
        write_byte(O_IO+IO_IR, read_byte(O_IO+IO_IR) | INT_TIM);
      }
    }
  }
  if (dmatimer != 0) {
    dmatimer -= dt;
    if (dmatimer <= 0) {
      dmatimer = 0;
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
      memory[O_IO + IO_JOYP] = (joyspecial & 0x0f) | (memory[O_IO + IO_JOYP] & 0xf0);
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
      memory[O_IO + IO_JOYP] = (joydirection & 0x0f) | (memory[O_IO + IO_JOYP] & 0xf0);
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
