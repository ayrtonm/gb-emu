#include "globals.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

mem::mem(string filename)
{
  load_cart(filename);
  io.at(_TIMA) = 0x00;
  io.at(_TMA) = 0x00;
  io.at(_TAC) = 0x00;
  io.at(_LCDC) = 0x91;
  io.at(_SCY) = 0x00;
  io.at(_SCX) = 0x00;
  io.at(_LYC) = 0x00;
  io.at(_BGP) = 0xfc;
  io.at(_OBP0) = 0xff;
  io.at(_OBP1) = 0xff;
  io.at(_WY) = 0x00;
  io.at(_WX) = 0x00;
  cout << "memory initialized\n";
}
uint8 mem::read_byte(uint16 address) const
{
#ifdef DEBUG
  cout << "reading from [0x" << hex << (int) address << "]\n";
#endif
  switch(address & 0xf000)
  {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000: {return romb0.at(address);}
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000: {return rombn.at(address - _ROMBN);}//this is temporary, eventually will add offset for mbc
    case 0x8000:
    case 0x9000: {return vram.at(address - _M_VRAM);}
    case 0xa000:
    case 0xb000: {return eram.at(address - _ERAM);}//this is temporary, eventually will add offset for mbc
    case 0xc000: {return wramb0.at(address - _WRAM0);}
    case 0xd000: {return wramb1.at(address - _WRAM1);}
    case 0xe000: {return wramb0.at(address - _ECHO0);}
    case 0xf000:
    {
//      assert(!(_UNUSED <= address <= 0xfeff));
      if (address < _M_OAM) {return wramb1.at(address - _ECHO1);}
      else if (address < _UNUSED) {return oam.at(address - _M_OAM);}
      else if (address < _IO) {return 0;}//unused section of memory returns 0
      else if (address < _HRAM) {return io.at(address - _IO);}
      else if (address <= _HRAM_END) {return hram.at(address - _HRAM);}
      else {return interrupt_enable;}
    }
  }
}
uint16 mem::read_word(uint16 address) const
{
  return (read_byte(address))+(read_byte(address + 1) << 8);
}
void mem::write_byte(uint16 address, uint8 data)
{
#ifdef DEBUG
  cout << "writing to [0x" << hex << (int) address << "]\n";
#endif
  switch(address & 0xf000)
  {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000: {break;}
    case 0x8000:
    case 0x9000: {vram.at(address - _M_VRAM) = data;break;}
    case 0xa000:
    case 0xb000: {eram.at(address - _ERAM) = data;break;}//this is temporary, eventually will add offset for mbc
    case 0xc000: {wramb0.at(address - _WRAM0) = data;break;}
    case 0xd000: {wramb1.at(address - _WRAM1) = data;break;}
    case 0xe000: {wramb0.at(address - _ECHO0) = data;break;}
    case 0xf000:
    {
//      assert(!(_UNUSED <= address <= 0xfeff));
      if (address < _M_OAM) {wramb1.at(address - _ECHO1) = data;break;}
      else if (address < _UNUSED) {oam.at(address - _M_OAM) = data;break;}
      else if (address < _IO) {break;}
      else if (address < _HRAM) {io.at(address - _IO) = data;update_io();break;}
      else if (address <= _HRAM_END) {hram.at(address - _HRAM) = data;break;}
      else {interrupt_enable = data & 0x1f;break;}
    }
  }
}
void mem::write_word(uint16 address, uint16 data)
{
  write_byte(address,data & 0x00ff);
  write_byte(address+1,data >> 8);
}
void mem::load_cart(string filename)
{
  streampos size;
  ifstream cart;
  //open file with pointer positioned at the end
  cart.open(filename, ios::binary|ios::ate);
  assert(cart.is_open());
  size = cart.tellg();
  rombn.resize(size - _ROMBN);
  cart.seekg(0,ios::beg);
  cart.read((char *) &romb0[0],_ROMBN);
  cart.read((char *) &rombn[0],size-_ROMBN);
  cart.close();
  for (int i =0x0134; i < 0x0144; i++) cout << romb0.at(i);//print title
  cout << (romb0.at(0x014A) ? "\nNon-Japanese" : "\nJapanese");
  cout << "\n";
}
void mem::update_io(void)
{
  io.at(_IR) &= 0x1f;
  io.at(_LCDSTAT) &= 0x78;
  update_palette(2,io.at(_BGP));
  update_palette(0,io.at(_OBP0));
  update_palette(1,io.at(_OBP1));
}
//I should split this function up
void mem::update_palette(uint8 palette, uint8 value)
{
  if (palette == 0)
  {
    int j = 0;
    for (int i = 0x03; i < 0xff; i = i << 2)
    {
      switch (GET(i,value) >> (j << 1))
      {
        case 0: {pal_obp0.at(j) = SDL_MapRGB(format,0xff,0xff,0xff);break;}
        case 1: {pal_obp0.at(j) = SDL_MapRGB(format,0xc0,0xc0,0xc0);break;}
        case 2: {pal_obp0.at(j) = SDL_MapRGB(format,0x60,0x60,0x60);break;}
        case 3: {pal_obp0.at(j) = SDL_MapRGB(format,0x00,0x00,0x00);break;}
      }
      j++;
    }
  }
  if (palette == 1)
  {
    int j = 0;
    for (int i = 0x03; i < 0xff; i = i << 2)
    {
      switch (GET(i,value) >> (j << 1))
      {
        case 0: {pal_obp1.at(j) = SDL_MapRGB(format,0xff,0xff,0xff);break;}
        case 1: {pal_obp1.at(j) = SDL_MapRGB(format,0xc0,0xc0,0xc0);break;}
        case 2: {pal_obp1.at(j) = SDL_MapRGB(format,0x60,0x60,0x60);break;}
        case 3: {pal_obp1.at(j) = SDL_MapRGB(format,0x00,0x00,0x00);break;}
      }
      j++;
    }
  }
  if (palette == 2)
  {
    int j = 0;
    for (int i = 0x03; i < 0xff; i = i << 2)
    {
      switch (GET(i,value) >> (j << 1))
      {
        case 0: {pal_bgp.at(j) = SDL_MapRGB(format,0xff,0xff,0xff);break;}
        case 1: {pal_bgp.at(j) = SDL_MapRGB(format,0xc0,0xc0,0xc0);break;}
        case 2: {pal_bgp.at(j) = SDL_MapRGB(format,0x60,0x60,0x60);break;}
        case 3: {pal_bgp.at(j) = SDL_MapRGB(format,0x00,0x00,0x00);break;}
      }
      j++;
    }
  }
}
