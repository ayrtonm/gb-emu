#include "mem.h"
#include "globals.h"
#include <stdio.h>//open and load file
#include <stdlib.h>//allocate memory
#include <sys/stat.h>//get file size

uint8 *load_cart(char *filename)
{
  struct stat st;
  stat(filename,&st);
  int size = st.st_size;
  uint8 *cart;
  cart = malloc(size*sizeof(uint8));
  FILE *ptr = fopen(filename,"rb");
  fread(cart,size*sizeof(uint8),1,ptr);
  fclose(ptr);
  return cart;
}

mbc parse_header(uint8 *cart)
{
  int i;
  for (i = 0x0134; i < 0x0144; i++) printf("%c",cart[i]);//print title
  printf("\nROM Size: %d bytes", rom_sizes[cart[0x0148] & 0x07] << 8);
  printf("\nRAM Size: %d bytes", ram_sizes[cart[0x0149]] << 8);
  if (cart[0x014A] == 0) printf("\nJapanese");
  else printf("\nNon-Japanese");
  char *type;
  int version = cart[0x0147];
  switch(version)
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
  mbc m;
  m.cart = cart;
  m.version = version;
  m.rombank = 0;
  m.rambank = 0;
  m.mode = 0;
  m.enable = 0;
  return m;
}

mem init_mem(void)
{
  int i;
  mem m;
  for (i = _VRAM; i < 0x010000; i++) m.map[i] = 0;
  for (i = 0; i < _VRAM; i++) m.map[i] = CART(i);
  write_byte(_LCDC,0x91);
  write_byte(_BGP,0xFC);
  write_byte(_OBP0,0xFF);
  write_byte(_OBP1,0xFF);
  return m;
}

void write_byte(uint16 address, uint8 value)
{
  MEM(address) = value;
}
void write_cart(uint16 address, uint8 value)
{

}
void write_word(uint16 address, uint16 value)
{
  write_byte(address,value & 0x00FF);
  write_byte(address+1,value >> 8);
}
