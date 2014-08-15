#include "mem.h"
//get file size(s)
#include <sys/stat.h>

/**
  open rom file and return data as an array of uint8
**/
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

/**
  read cartridge header, print relevant information and return mbc struct. eventually memory for external ram will be allocated here
**/
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

/**
  load cartridge rom bank 0/1 and reset io registers to default values and clear the rest
  memory in the actual hw is random when it's reset, but this function sets everything that's not initialized to 0 like most other emulators
**/
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

/**
  should be self-explanatory
**/
void write_byte(uint16 address, uint8 value)
{
  //used for debugging//////////////////
  if (printing == memory) printf("[%x] = 0x%x\n",address,value);
  //////////////////////////////////////
  if (address < _BANK) {write_cart(address,value);}
  else if (address < _VRAM) {write_cart(address,value);}
  else if (address < _ERAM) {MEM(address) = value;}
  else if (address < _WRAM) {write_cart(address,value);}
  else if (address < _ECHO) {MEM(address) = value;}
  else if (address < _OAM) {MEM(address) = value;address -= 0x2000;write_byte(address,value);}
  else if (address < _UNUSED) {MEM(address) = value;}
  else if (address < _IO) {MEM(address) = value;}
  else if (address < _HRAM)
  {
    if (address == _JOYP)//filter out writing to bits 0-3,6,7
    {
      IO(_JOYP) = value|0xCF;
      if (!(value & 0x10))
      {
        if (gameboy->key_bitmap & UP_K) IO(_JOYP) &= ~0x04;
        if (gameboy->key_bitmap & DOWN_K) IO(_JOYP) &= ~0x08;
        if (gameboy->key_bitmap & LEFT_K) IO(_JOYP) &= ~0x02;
        if (gameboy->key_bitmap & RIGHT_K) IO(_JOYP) &= ~0x01;
      }
      if (!(value & 0x20))
      {
        if (gameboy->key_bitmap & A_K) IO(_JOYP) &= ~0x01;
        if (gameboy->key_bitmap & B_K) IO(_JOYP) &= ~0x02;
        if (gameboy->key_bitmap & STA_K) IO(_JOYP) &= ~0x08;
        if (gameboy->key_bitmap & SEL_K) IO(_JOYP) &= ~0x04;
      }
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
    else if (address == _IR) {IO(_IR) = value & 0x1F;}//request interrupt only bits 0-4 used
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
  else if (address < _IE) {MEM(address) = value;}
  else if (address == _IE) {MEM(address) = value & 0x1F;}
}

/**
  chooses different functions depending on mbc version from mbc.c to handle writing to cartridge area
**/
void write_cart(uint16 address, uint8 value)
{
  if (VERSION == 0)//ROM ONLY
    {return;}
  else if (0x08 <= VERSION <= 0x09) //ROM + RAM
    {write_ram(address,value);}
  else if (0x01 <= VERSION <= 0x03)//MBC1
    {write_mbc1(address,value);}
  else if (0x0F <= VERSION <= 0x13)//MBC3
    {write_mbc3(address,value);}
  else if (0x19 <= VERSION <= 0x1E )//MBC5
    {write_mbc5(address,value);}
}

/**
  should be self-explanatory
**/
void write_word(uint16 address, uint16 value)
{
  write_byte(address,LOW(value));
  write_byte(address+1,HIGH(value));
}
