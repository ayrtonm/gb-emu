#ifndef GLOBALS_H
#define GLOBALS_H

//general bit manipulation
#define BIT(x)		(1 << (x))
#define GET(a,b)	((a) & (b))
#define SET(a,b)	((b) |= (a))
#define CLEAR(a,b)	((b) &= ~(a))
#define TOGGLE(a,b)	((b) ^= (a))
#define SWAP(x)		(((x) & 0xF0) >> 4 | ((x) & 0x0F) << 4) //only for uint8
#define SIZE(x)		((sizeof(x)<<3)) //size in bits
#define LASTBIT(x)	((SIZE(x))-1) //'address' of last bit
#define CHECKNEG(x)	(GET(LASTBIT(x),x) == 0 ? 0 : 1) //returns value of last bit of x
#define LOW(x)		((x) & 0xFF)
#define HIGH(x)		((x) >> 8)

//typedefs and structs
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef union
{
  uint16 W;
  struct
  {
    #ifdef HIGH_ENDIAN
    uint8 h,l;
    #else
    uint8 l,h;
    #endif
  } B;
} word16;

typedef struct cpu
{
  word16 AF,BC,DE,HL,PC,SP;
  uint8 IME;
} cpu;

typedef struct mbc
{
  int version;
  uint8 rombank,rambank,mode,enable;
  uint8 *cart;
} mbc;

typedef struct mem
{
  uint8 map[0x010000];
  uint8 tile_data_0[0x0C00];
  uint8 tile_data_1[0x0C00];
  uint8 tile_map_0[0x0400];
  uint8 tile_map_1[0x0400];
  uint8 oam[0xA0];
  uint8 io[0x80];
} mem;

typedef struct gb
{
  cpu cpu;
  mbc mbc;
  mem mem;
} gb;

//global gb struct pointer
//only use any of the following macros after
//this pointer has been initialized
gb *gameboy;

//cpu specific
//flags
#define Z_FLAG	0x80
#define N_FLAG	0x40
#define H_FLAG	0x20
#define C_FLAG	0x10
#define CARRY	((GET(C_FLAG,_F)) >> 4)
#define ZERO	((GET(Z_FLAG,_F)) >> 7)

//registers
#define _A	(gameboy->cpu.AF.B.h)
#define _F	(gameboy->cpu.AF.B.l)
#define _B	(gameboy->cpu.BC.B.h)
#define _C	(gameboy->cpu.BC.B.l)
#define _D	(gameboy->cpu.DE.B.h)
#define _E	(gameboy->cpu.DE.B.l)
#define _H	(gameboy->cpu.HL.B.h)
#define _L	(gameboy->cpu.HL.B.l)
#define _AF	(gameboy->cpu.AF.W)
#define _BC	(gameboy->cpu.BC.W)
#define _DE	(gameboy->cpu.DE.W)
#define _HL	(gameboy->cpu.HL.W)
#define _SP	(gameboy->cpu.SP.W)
#define _PC	(gameboy->cpu.PC.W)
#define _PCBl	(gameboy->cpu.PC.B.l)
#define _PCBh	(gameboy->cpu.PC.B.h)
#define _IME	(gameboy->cpu.IME)

//immediate memory
#define IMM8	(READ_BYTE(_PC-1))
#define IMM16	(READ_WORD(_PC-2))

//memory shortcuts
#define MEM(x)		(gameboy->mem.map[x])
#define T_DATA_0(x)	(gameboy->mem.tile_data_0[x])
#define T_DATA_1(x)	(gameboy->mem.tile_data_1[x])
#define T_MAP_0(x)	(gameboy->mem.tile_map_0[x])
#define T_MAP_1(x)	(gameboy->mem.tile_map_1[x])
#define IO(x)		(gameboy->mem.io[x])
#define OAM(x)		(gameboy->mem.oam[x])

//mbc shortcuts
#define CART(x)		(gameboy->mbc.cart[x])
#define VERSION		(gameboy->mbc.version)
#define ROMBANK		(gameboy->mbc.rombank)
#define RAMBANK		(gameboy->mbc.rambank)
#define ENABLE		(gameboy->mbc.enable)
#define MODE		(gameboy->mbc.mode)

//read memory functions
#define READ_BYTE(x)	(MEM(x))
#define READ_WORD(x)	(MEM(x) + (MEM(x+1)<<8))

//memory sections
#define _ROM	0x0000
#define _BANK	0x4000
#define _VRAM	0x8000
#define _ERAM	0xA000
#define _WRAM	0xC000
#define _ECHO	0xE000
#define _OAM	0xFE00
#define _UNUSED	0xFEA0
#define _IO	0xFF00
#define _HRAM	0xFF80

//IO register addresses
#define _JOYP		0xFF00
#define _DIV		0xFF04
#define _TIMA		0xFF05
#define _TMA		0xFF06
#define _TAC		0xFF07
#define _LCDC		0xFF40
#define _LCDSTAT	0xFF41
#define _SCY		0xFF42
#define _SCX		0xFF43
#define _LY		0xFF44
#define _LYC		0xFF45
#define _DMA		0xFF46
#define _BGP		0xFF47
#define _OBP0		0xFF48
#define _OBP1		0xFF49
#define _WY		0xFF4A
#define _WX		0xFF4B
#define _IF		0xFF0F
#define _IE		0xFFFF

#endif
