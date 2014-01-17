#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "mem.h"
#include "input.h"

uint8 *load_cart(char *filename)
{
  struct stat st;
  stat(filename, &st);
  int size = st.st_size;
  cart = malloc(size*sizeof(uint8));
  FILE *ptr = fopen(filename, "rb");
  fread(cart,size*sizeof(uint8),1,ptr);
  fclose(ptr);
  return cart;
}
int print_header(char *cart)
{
  int i;
  for(i=0x0134;i<0x144;i++) printf("%c",cart[i]);
  printf("\nROM Size: %d bytes", rom_sizes[cart[0x0148] & 0x07] << 8);
  printf("\nRAM Size: %d bytes", ram_sizes[cart[0x0149]] << 8);
  if (cart[0x014A] == 0) {printf("\nJapanese");}
  else {printf("\nNon-Japanese");}
  char *type;
  int mbc = cart[0x0147];
  switch(mbc)
  {
    case 0x00: type = "ROM ONLY";break;
    case 0x01: type = "MBC1";break;
    case 0x02: type = "MBC1+RAM";break;
    case 0x03: type = "MBC1+RAM+BATTERY";break;
    case 0x05: type = "MBC2";break;
    case 0x06: type = "MBC2+BATTERY";break;
    case 0x08: type = "ROM+RAM";break;
    case 0x09: type = "ROM+RAM+BATTERY";break;
    case 0x0B: type = "MMM01";break;
    case 0x0C: type = "MMM01+RAM";break;
    case 0x0D: type = "MMM01+RAM+BATTERY";break;
    case 0x0F: type = "MBC3+TIMER+BATTERY";break;
    case 0x10: type = "MBC3+TIMER+RAM+BATTERY";break;
    case 0x11: type = "MBC3";break;
    case 0x12: type = "MBC3+RAM";break;
    case 0x13: type = "MBC3+RAM+BATTERY";break;
    case 0x15: type = "MBC4";break;
    case 0x16: type = "MBC4+RAM";break;
    case 0x17: type = "MBC4+RAM+BATTERY";break;
    case 0x19: type = "MBC5";break;
    case 0x1A: type = "MBC5+RAM";break;
    case 0x1B: type = "MBC5+RAM+BATTERY";break;
    case 0x1C: type = "MBC5+RUMBLE";break;
    case 0x1D: type = "MBC5+RUMBLE+RAM";break;
    case 0x1E: type = "MBC5+RUMBLE+RAM+BATTERY";break;
    default: type = "OTHER";break;
  }
  printf("\n%s\n",type);
  for (i=0;i<_VRAM;i++)
    {mem[i]=cart[i];}
  return mbc;
}
uint8 read_byte(uint16 address)
{
  char string[35];
  snprintf(string,35,"  read from [0x%x]\n",address);
  write_log(string);
  if (_ECHO <= address && address < _OAM) {return mem[address - 0x2000];}
  else if (address == _DMA) {return 0;}
  else if (address == _JOYP) //{return rows[(column>>4)-1];} this should give same output as if else if statements
    {
      if (column == 0x10) {return rows[0];}
      else if (column == 0x20) {return rows[1];}
    }
  else if (address == _DIV) {return divr;}
  else if (address == _TIMA) {return tima;}
  else if (address == _TMA) {return tma;}
  else if (address == _TAC) {return tac;}
  else if (address == _LCDC) {return lcdc;}
  else if (address == _LCDSTAT) {return lcdstat;}
  else if (address == _SCY) {return scy;}
  else if (address == _SCX) {return scx;}
  else if (address == _LY) {return line;}
  else if (address == _LYC) {return lyc;}
  else if (address == _BGP) {return bgp;}
  else if (address == _OBP0) {return obp0;}
  else if (address == _OBP1) {return obp1;}
  else if (address == _WY) {return wy;}
  else if (address == _WX) {return wx;}
  else if (address == _IF) {return iff;}
  else if (address == _IE) {return ie;}
  else {return mem[address];}
}
void write_byte(uint16 address, uint8 value)
{
  char string[45];
  snprintf(string,45,"  wrote 0x%x to [0x%x]\n",value,address);
  write_log(string);
  if (_ECHO <= address && address < _OAM) {mem[address - 0x2000] = value;}
  else if (address == _DMA && value < 0xF2)
    {int i;value = value << 8;
     for (i=value;i<value+0xA0;i++)
     {write_byte(mem[i-value+_OAM],mem[i]);}
    }
  //write only to bits 4 and 5
  else if (address == _JOYP) {column = (value & 0x30);}
  else if (address == _DIV) {divr = value;}
  else if (address == _TIMA) {tima = value;}
  else if (address == _TMA) {tma = value;}
  else if (address == _TAC) {tac = value;}
  else if (address == _LCDC) {lcdc = value;}
  else if (address == _LCDSTAT) {lcdstat = value;}
  else if (address == _SCY) {scy = value;}
  else if (address == _SCX) {scx = value;}
  else if (address == _LY) {}
  else if (address == _LYC) {lyc = value;}
  else if (address == _BGP) {bgp = value;update_palette(2,bgp);}
  else if (address == _OBP0) {obp0 = value;update_palette(0,obp0);}
  else if (address == _OBP1) {obp1 = value;update_palette(1,obp1);}
  else if (address == _WY) {wy = value;}
  else if (address == _WX) {wx = value;}
  else if (address == _IF) {iff = value;}
  else if (address == _IE) {ie = value;}
  else if (address < _VRAM) {value = 0;}
  else {mem[address] = value;}
}
uint16 read_word(uint16 address)
{
  read_byte(address) + (read_byte(address+1)<<8);
}
void write_word(uint16 address, uint16 value)
{
  write_word(address, value & 0xFF);
  write_word(address+1, value >> 8);
}