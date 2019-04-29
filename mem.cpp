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
  memory[O_IO + IO_LCDC] = 0x91;
  memory[O_IO + IO_SCY] = 0x00;
  memory[O_IO + IO_SCX] = 0x00;
  memory[O_IO + IO_LYC] = 0x00;
  memory[O_IO + IO_BGP] = 0xfc;
  memory[O_IO + IO_OBP0] = 0xff;
  memory[O_IO + IO_OBP1] = 0xff;
  memory[O_IO + IO_WY] = 0x00;
  memory[O_IO + IO_WX] = 0x00;
  //initialized to 0x91 so lcd mode is initially VBLANK(0x01)
  memory[O_IO + IO_LCDSTAT] = 0x91;
  //initializing the following to 0x00 to prevent warnings
  memory[O_IO + IO_JOYP] = 0x00;
  memory[O_IO + IO_DIV] = 0x00;
  memory[O_IO + IO_IR] = 0x00;
  memory[O_IO + IO_LY] = 0x00;
  memory[O_IO + IO_DMA] = 0x00;
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
  cout << "\n";
}
void mem::update_io(void)
{
  update_palette(2,memory[O_IO + IO_BGP]);
  update_palette(0,memory[O_IO + IO_OBP0]);
  update_palette(1,memory[O_IO + IO_OBP1]);
  //since the palette has changed, the following lines
  //request an interrupt to update the screen
  memory[O_IO + IO_IR] &= 0x1f;
  memory[O_IO + IO_LCDSTAT] &= 0x78;
}
//I should split this function up
void mem::update_palette(uint8 palette, uint8 value)
{
  //value &= 0xfc;
  int j = 0;
  for (int i = 0x03; i < 0xff; i = i << 2)
  {
    switch (GET(i,value) >> (j << 1))
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
