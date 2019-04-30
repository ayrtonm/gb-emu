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
  memory[O_IO + IO_LCDSTAT] = 0x79;
  divtimer = 0;
  timatimer = 0;
  tacthreshold = 1024;
  dmatimer = 0;
  //0x0f is no keys pressed
  joydirection = 0x0f;
  joyspecial = 0x0f;
  //initiallize joypad register to something sensible
  //memory[O_IO + IO_JOYP] = 0x20 + joydirection;
  cout << "memory initialized\n";
}

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
  //sprite palettes don't use lowest two bits so let's keep them zeroed out
  if (palette != 2) {value &= 0xfc;}
  int j = 0;
  for (int i = 0x03; i < 0xff; i = i << 2)
  {
    switch ((value & i) >> (2*j))
    {
      case 0: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0xc0,0xc0,0xc0};break;}
      case 1: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0x80,0x80,0x80};break;}
      case 2: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0x60,0x60,0x60};break;}
      case 3: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0x30,0x30,0x30};break;}
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
  if (divtimer > 256) {
    divtimer -= 256;
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
  if (special) {
    if (down) {
      joyspecial &= ~bit;
    }
    else {
      joyspecial |= bit;
    }
    //update addressable memory special keys are currently selected
    //this way memory is kept up to date without having to check in read_byte
    if ((memory[O_IO + IO_JOYP] & JOYP_SPECIAL_SELECTED) == 0x00) {
      memory[O_IO + IO_JOYP] = joyspecial + (memory[O_IO + IO_JOYP] & 0xf0);
    }
  }
  else {
    if (down) {
      joydirection &= ~bit;
    }
    else {
      joydirection |= bit;
    }
    if ((memory[O_IO + IO_JOYP] & JOYP_DIRECTION_SELECTED) == 0x00) {
      memory[O_IO + IO_JOYP] = joydirection + (memory[O_IO + IO_JOYP] & 0xf0);
    }
  }
}
uint8 mem::get_keys(bool special) {
  if (special) {
    return joyspecial;
  }
  else {
    return joydirection;
  }
}
