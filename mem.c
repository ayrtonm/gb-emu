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
  m.rombank.W = 0;
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
  m.map[_TIMA] = 0x00;
  m.map[_TMA] = 0x00;
  m.map[_TAC] = 0x00;
  m.map[_LCDC] = 0x91;
  m.map[_SCY] = 0x00;
  m.map[_SCX] = 0x00;
  m.map[_LY] = 0x00;
  m.map[_LYC] = 0x00;
  m.map[_WY] = 0x00;
  m.map[_WX] = 0x00;
  m.map[_IE] = 0x00;
  m.map[_BGP] = 0xFC;
  m.map[_OBP0] = 0xFF;
  m.map[_OBP1] = 0xFF;
  update_palette(0,0xFF);
  update_palette(1,0xFF);
  update_palette(2,0xFC);
  return m;
}

void write_byte(uint16 address, uint8 value)
{
  if (printing == memory) printf("[%x] = 0x%x\n",address,value);
  //write to rom
  if (address < _BANK) {write_cart(address,value);}
  //write to external rombank
  else if (address < _VRAM) {write_cart(address,value);}
  //write to vram(unfinished)
  else if (address < _ERAM) {MEM(address) = value;}
  //write to external rambank
  else if (address < _WRAM) {write_cart(address,value);}
  //write to wram
  else if (address < _ECHO) {MEM(address) = value;}
  //write to echo space then copy to wram
  else if (address < _OAM) {MEM(address) = value;address -= 0x2000;write_byte(address,value);}
  //write to oam
  else if (address < _UNUSED) {MEM(address) = value;}
  //shouldn't actually be used
  else if (address < _IO) {MEM(address) = value;}
  //write to io hw registers
  else if (address < _HRAM)
  {
    if (address == _JOYP)//filter out writing to bits 0-3,6,7
    {
//      IO(_JOYP) = value & 0x30;
      if (value & 0x10) {IO(_JOYP) |= 0x10;IO(_JOYP) &= ~0x20;}
      else if (value & 0x20) {IO(_JOYP) |= 0x20;IO(_JOYP) &= ~0x10;}
      else if (value & 0x30) {IO(_JOYP) = 0x10;IO(_JOYP) &= ~0x20;}//what ACTUALLY happens when both lines are on???
    }
    else if (address == _DIV) {IO(_DIV) = 0;}//write any value resets div register
    else if (address == _TIMA) {IO(_TIMA) = value;}
    else if (address == _TMA) {IO(_TMA) = value;}
    else if (address == _TAC)//bit 2 - start/stop timer,bit 0-1 - input clock select
    {
      IO(_TAC) = value & 0x07;
      switch(IO(_TAC) & 0x03)
      {
        case 0x00: {gameboy->time_period = gameboy->time_clk = T_TIMER_0;break;}
        case 0x01: {gameboy->time_period = gameboy->time_clk = T_TIMER_1;break;}
        case 0x02: {gameboy->time_period = gameboy->time_clk = T_TIMER_2;break;}
        case 0x03: {gameboy->time_period = gameboy->time_clk = T_TIMER_3;break;}
      }
    }
    else if (address == _IF) {IO(_IF) = value & 0x1F;}//request interrupt only bits 0-4 used
    else if (address == _LCDC) {IO(_LCDC) = value;}//all can be controlled by user
    else if (address == _LCDSTAT) {IO(_LCDSTAT) = value & 0x78;}//bits 3-6 enable different interrupts, 0-2 read only
    else if (address == _SCY) {IO(_SCY) = value;}//scroll y
    else if (address == _SCX) {IO(_SCX) = value;}//scroll x
    else if (address == _LY) {return;}//vertical line currently being used read only
    else if (address == _LYC) {IO(_LYC);}//"ly compare" arbitrary register that is always compared to ly register
    else if (address == _DMA && value <= 0xF1)//direct memory access transfer
      {int i; for (i = 0; i < 0xA0; i++) {write_byte(0xFE00 + i, READ_BYTE((value << 8) + i));}}
    else if (address == _BGP) {IO(_BGP) = value;update_palette(2,value);}//background palette
    else if (address == _OBP0) {IO(_OBP0) = value;update_palette(0,value);}//obj0 palette
    else if (address == _OBP1) {IO(_OBP1) = value;update_palette(1,value);}//obj1 palette
    else if (address == _WY) {IO(_WY) = value;}//window y position
    else if (address == _WX) {IO(_WX) = value;}//window x position
  }
  //write to hram(stack)
  else if (address < _IE) {MEM(address) = value;}
}
void write_cart(uint16 address, uint8 value)
{
  if (VERSION == 0) {return;} //ROM ONLY
  else if (VERSION == 0x08 || VERSION == 0x09) //ROM + RAM (+ BATTERY)
  {
    if (_ERAM <= address < _WRAM) {MEM(address) = value;}
  }
  else if (VERSION == 0x01 || VERSION == 0x02 || VERSION == 0x03) //MBC1
  {
    if (address < 0x2000) {ENABLE = ((value & 0x0F) == 0x0A) ? 1 : 0;}
    else if (address < 0x4000) {ROMBANK.B.l = value & 0x1F;if (ROMBANK.B.l == 0) {ROMBANK.B.l = 0x01;}ROMBANK.W &= 0x031F;int i;for (i = _ROM; i < _BANK; i++) {MEM(i + _BANK) = CART(i+(ROMBANK.B.l + (ROMBANK.B.h << 5))*0x4000);}}
    else if (address < 0x6000)
    {
      if (MODE == 0) {ROMBANK.B.h = value & 0x03;ROMBANK.W &= 0x031F;int i;for (i = _ROM; i < _BANK; i++) {MEM(i + _BANK) = CART(i + (ROMBANK.B.l + (ROMBANK.B.h << 5))*0x4000);}}
      else if (MODE == 1) {uint8 oldbank = RAMBANK; RAMBANK = value & 0x03;int i;for (i = _ERAM; i < _WRAM; i++) {ERAM((i - _ERAM) + oldbank*0x2000) = MEM(i); MEM(i) = ERAM((i - _ERAM) + RAMBANK*0x2000);}}
    }
    else if (address < 0x8000) {MODE = value & 0x01;}
    else if (_ERAM <= address < _WRAM) {MEM(address) = (ENABLE == 1) ? value : MEM(address);}
  }
  else if (VERSION == 0x0F || VERSION == 0x10 || VERSION == 0x11 || VERSION == 0x12 || VERSION == 0x13) //MBC3 for Pokemon Oro
  {
    if (address < 0x2000) {ENABLE = ((value & 0x0F) == 0x0A) ? 1 : 0;}
    else if (address < 0x4000) {ROMBANK.B.l = value & 0x7F;if (ROMBANK.B.l == 0) {ROMBANK.B.l = 0x01;}ROMBANK.W &= 0x007F;int i;for (i = _ROM; i < _BANK; i++) {MEM(i + _BANK) = CART(i+ROMBANK.B.l*0x4000);}}
    else if (address < 0x6000)
    {
      if (value < 0x04) {uint8 oldbank = RAMBANK; RAMBANK = value & 0x03;int i;for (i = _ERAM; i < _WRAM; i++) {ERAM((i - _ERAM) + oldbank*0x2000) = MEM(i); MEM(i) = ERAM((i - _ERAM) + RAMBANK*0x2000);}}
      else if (value < 0x0C) {return;}//map corresponding rtc register to memory at a000-bfff, rtc register can be accessed by writing to any address in range
    }
    else if (address < 0x8000) {return;}//latch clock data by writing 0x00 then 0x01
    else if (_ERAM <= address < _WRAM) {MEM(address) = (ENABLE == 1) ? value : MEM(address);}
  }
  else if (VERSION == 0x19 ||VERSION == 0x1A || VERSION == 0x1B ||VERSION == 0x1C || VERSION == 0x1E) //MBC5 for Pokemon Azul and Pokemon Amarillo
  {
    if (address < 0x2000) {ENABLE = ((value & 0x0F) == 0x0A) ? 1 : 0;}
    else if (address < 0x3000) {ROMBANK.B.l = value;ROMBANK.W &= 0x01FF;int i;for (i = _ROM; i < _BANK; i++) {MEM(i + _BANK) = CART((i+ROMBANK.W*0x4000) & ((rom_sizes[CART(0x0148)] << 8) - 1));}}
    else if (address < 0x4000) {ROMBANK.B.h = value & 0x01;ROMBANK.W &= 0x01FF;int i;for (i = _ROM; i < _BANK; i++) {MEM(i + _BANK) = CART((i+ROMBANK.W*0x4000) & ((rom_sizes[CART(0x0148)] << 8) - 1));}}
    else if (address < 0x6000) {uint8 oldbank = RAMBANK; RAMBANK = value & 0x0F;int i;for (i = _ERAM; i < _WRAM; i++) {ERAM((i-_ERAM) + oldbank*0x2000) = MEM(i);MEM(i) = ERAM((i - _ERAM) + RAMBANK*0x2000);}}
    else if (address < 0x8000) {return;}
    else if (_ERAM <= address < _WRAM) {MEM(address) = (ENABLE == 1) ? value : MEM(address);}
  }
}
void write_word(uint16 address, uint16 value)
{
  write_byte(address,LOW(value));
  write_byte(address+1,HIGH(value));
}
