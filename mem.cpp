#include "globals.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

mem::mem(string filename)
{
  load_cart(filename);
  io.at(IO_TIMA) = 0x00;
  io.at(IO_TMA) = 0x00;
  io.at(IO_TAC) = 0x00;
  io.at(IO_LCDC) = 0x91;
  io.at(IO_SCY) = 0x00;
  io.at(IO_SCX) = 0x00;
  io.at(IO_LYC) = 0x00;
  io.at(IO_BGP) = 0xfc;
  io.at(IO_OBP0) = 0xff;
  io.at(IO_OBP1) = 0xff;
  io.at(IO_WY) = 0x00;
  io.at(IO_WX) = 0x00;
  //initialized to 0x91 so lcd mode is initially VBLANK(0x01)
  io.at(IO_LCDSTAT) = 0x91;
  //initializing the following to 0x00 to prevent warnings
  io.at(IO_JOYP) = 0x00;
  io.at(IO_DIV) = 0x00;
  io.at(IO_IR) = 0x00;
  io.at(IO_LY) = 0x00;
  io.at(IO_DMA) = 0x00;
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
    case 0x7000: {return rombn.at(address - O_ROMBN);}//this is temporary, eventually will add offset for mbc
    case 0x8000:
    case 0x9000: {return vram.at(address - O_VRAM);}
    case 0xa000:
    case 0xb000: {return eram.at(address - O_ERAM);}//this is temporary, eventually will add offset for mbc
    case 0xc000: {return wramb0.at(address - O_WRAM0);}
    case 0xd000: {return wramb1.at(address - O_WRAM1);}
    case 0xe000: {return wramb0.at(address - O_ECHO0);}
    case 0xf000:
    {
//      assert(!(O_UNUSED <= address <= 0xfeff));
      if (address < O_OAM) {return wramb1.at(address - O_ECHO1);}
      else if (address < O_UNUSED) {return oam.at(address - O_OAM);}
      else if (address < O_IO) {return 0;}//unused section of memory returns 0
      else if (address < O_HRAM) {return io.at(address - O_IO);}
      else if (address <= O_HRAM_END) {return hram.at(address - O_HRAM);}
      else {return interrupt_enable;}
    }
  }
  return interrupt_enable;
}
uint16 mem::read_word(uint16 address) const
{
  return (read_byte(address))+(read_byte(address + 1) << 8);
}
void mem::write_byte(uint16 address, uint8 data)
{
#ifdef DEBUG
  cout << "writing 0x" << hex << (int) data << " to [0x" << hex << (int) address << "]\n";
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
    case 0x9000: {vram.at(address - O_VRAM) = data;break;}
    case 0xa000:
    case 0xb000: {eram.at(address - O_ERAM) = data;break;}//this is temporary, eventually will add offset for mbc
    case 0xc000: {wramb0.at(address - O_WRAM0) = data;break;}
    case 0xd000: {wramb1.at(address - O_WRAM1) = data;break;}
    case 0xe000: {wramb0.at(address - O_ECHO0) = data;break;}
    case 0xf000:
    {
//      assert(!(O_UNUSED <= address <= 0xfeff));
      if (address < O_OAM) {wramb1.at(address - O_ECHO1) = data;break;}
      else if (address < O_UNUSED) {oam.at(address - O_OAM) = data;break;}
      else if (address < O_IO) {break;}
      else if (address < O_HRAM) {update_io();io.at(address - O_IO) = data;break;}
      else if (address <= O_HRAM_END) {hram.at(address - O_HRAM) = data;break;}
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
  rombn.resize((int)size - (int)O_ROMBN);
  cart.seekg(0,ios::beg);
  cart.read((char *) &romb0[0],O_ROMBN);
  cart.read((char *) &rombn[0],(int)size-(int)O_ROMBN);
  cart.close();
  for (int i =0x0134; i < 0x0144; i++) cout << romb0.at(i);//print title
  cout << (romb0.at(0x014A) ? "\nNon-Japanese" : "\nJapanese");
  cout << "\n";
}
void mem::update_io(void)
{
  io.at(IO_IR) &= 0x1f;
  io.at(IO_LCDSTAT) &= 0x78;
  update_palette(2,io.at(IO_BGP));
  update_palette(0,io.at(IO_OBP0));
  update_palette(1,io.at(IO_OBP1));
}
//I should split this function up
void mem::update_palette(uint8 palette, uint8 value)
{
  int j = 0;
  for (int i = 0x03; i < 0xff; i = i << 2)
  {
    switch (GET(i,value) >> (j << 1))
    {
      case 0: {palettes[palette].at(j) = SDL_MapRGB(format,0xff,0xff,0xff);break;}
      case 1: {palettes[palette].at(j) = SDL_MapRGB(format,0xc0,0xc0,0xc0);break;}
      case 2: {palettes[palette].at(j) = SDL_MapRGB(format,0x60,0x60,0x60);break;}
      case 3: {palettes[palette].at(j) = SDL_MapRGB(format,0x00,0x00,0x00);break;}
    }
    j++;
  }
}

array<Uint32,4> mem::get_palette(uint8 palette_num)
{
  return palettes[palette_num];
}
void mem::set_format(const SDL_PixelFormat *fmt)
{
  format = fmt;
}
void mem::dump_memory(string filename)
{
  ofstream dump;
  dump.open(filename);
  assert(dump.is_open());
  for (uint16 i = 0; i < 0xffff; i++) {
    if (read_byte(i) != 0) {
      dump << "[0x" << hex << i << "]  0x" << hex << (int)read_byte(i) << endl;
    }
  }
  dump.close();
}
