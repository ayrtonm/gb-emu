#include "mem.h"
#include "globals.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

uint8 *load_cart(char *filename)
{
  struct stat st;
  stat(filename, &st);
  int size = st.st_size;
  uint8 *cart;
  cart = malloc(size*sizeof(uint8));
  FILE *ptr = fopen(filename,"rb");
  fread(cart,size*sizeof(uint8),1,ptr);
  fclose(ptr);
  return cart;
}

mbc *parse_header(char *cartridge)
{
  int i;
  for (i = 0x0134; i < 0x0144; i++) printf("%c",cartridge[i]);
  printf("\nROM Size: %d bytes", rom_sizes[cartridge[0x0148] & 0x07] << 8);
  printf("\nRAM Size: %d bytes", ram_sizes[cartridge[0x0149]] << 8);
  if (cartridge[0x014A] == 0) printf("\nJapanese");
  else printf("\nNon-Japanese");
  char *type;
  int version = cartridge[0x0147];
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
  printf("\n%s",type);
  for (i = 0; i < _VRAM; i++) MEM(i) = cartridge[i];
  for (i = _IO; i < _HRAM; i++) MEM(i) = 0;
  for (i = _OAM; i < _UNUSED; i++) MEM(i) = 0;
  write_byte(_LCDC,0x91);
  write_byte(_BGP,0xFC);
  write_byte(_OBP0,0xFF);
  write_byte(_OBP1,0xFF);
  mbc *cart;
  cart->cart = cartridge;
  cart->version = version;
  cart->rombank = 0;
  cart->rambank = 0;
  cart->mode = 0;
  cart->enable = 0;
  return cart;
}

void write_byte(uint16 address, uint8 value)
{
  if (address < _BANK) {write_cart(address,value);}//write rom (mbc)
  else if (address < _VRAM) {write_cart(address,value);}//write bank (mbc)
  else if (address < _ERAM) {MEM(address) = value;}//write vram
  else if (address < _WRAM) {write_cart(address,value);}//write eram (mbc)
  else if (address < _ECHO) {MEM(address) = value;}//write wram
  else if (address < _OAM) {MEM(address) = value;}//write echo
  else if (address < _UNUSED) {MEM(address) = value;}//write oam
  else if (address < _IO) {MEM(address) = value;}//write unused
  else if (address < _HRAM) {MEM(address) = value;}//write IO
  else if (address < _IE) {MEM(address) = value;}//write hram
}

void write_cart(uint16 address, uint8 value)
{
  //modify mbc struct
  if (VERSION == 0) {return;}
  else if (VERSION == 1)
  {
    if (address < 0x2000) {ENABLE = value & 0x0F;}
    else if (address < 0x4000) {ROMBANK |= value & 0x1F;}
    else if (address < 0x6000)
    {
      if (MODE == 0) {ROMBANK |= (value & 0x03) << 5;}
      else if (MODE == 1) {RAMBANK = value & 0x03;}
    }
    else if (address < 0x8000) {MODE = value & 0x01;}
    else if (address < 0xC000) {}
    //update windows
    int i;
    for (i = _BANK; i < _VRAM; i++) MEM(i) = CART(i);
    return;
  }
}

void write_word(uint16 address, uint16 value)
{
  write_byte(address,(value & 0x00FF));
  write_byte(address+1,(value >> 8));
}