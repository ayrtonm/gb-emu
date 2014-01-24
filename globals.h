#ifndef GLOBALS_H
#define GLOBALS_H
//no local headers should be included here
//only sdl and possibly ncurses for debug mode ui
//most other header files should only include
//globals.h,stdio, and stdlib
#include <SDL/SDL.h>

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
} mem;

typedef struct lcd
{
  SDL_Surface *screen;
  uint8 linebuffer[160];
  int clk;
} lcd;

typedef struct gb
{
  cpu cpu;
  mbc mbc;
  mem mem;
  lcd lcd;
  int div_clk;
  int time_clk;
  int time_period;
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

//interrupts
#define REQUEST_INT(x)	write_byte(_IF,SET(x,IO(_IF)))
#define INT_VBL	0x01
#define INT_LCD	0x02
#define INT_TIM	0x04
#define INT_SER	0x08
#define INT_JOY	0x10

//lcd modes
#define MODE_HBLANK	0x00
#define MODE_VBLANK	0x01
#define MODE_OAM	0x02
#define MODE_VRAM	0x03
//get mode
#define LCD_MODE	(IO(_LCDSTAT) & 0x03)
//set mode
#define SET_MODE_HBLANK	CLEAR(0x03,IO(_LCDSTAT))
#define SET_MODE_VBLANK	SET(0x01,IO(_LCDSTAT));CLEAR(0x02,IO(_LCDSTAT))
#define SET_MODE_OAM	SET(0x02,IO(_LCDSTAT));CLEAR(0x01,IO(_LCDSTAT))
#define SET_MODE_VRAM	SET(0x03,IO(_LCDSTAT))

//timer shortcuts
#define TIMER_ON	(IO(_TAC) & 0x04)

//periods
//time for ly increment during vblank
#define T_LY_INC	((T_HBLANK)+(T_OAM)+(T_VRAM))
#define T_HBLANK	204
//T_VBLANK not used
#define T_VBLANK	4560
#define T_OAM		80
#define T_VRAM		172

//timer period modes are completely unrelated to lcd modes
#define T_TIMER_0	256	//4096 Hz
#define T_TIMER_1	4	//262144 Hz
#define T_TIMER_2	16	//65536 Hz
#define T_TIMER_3	64	//16384 Hz
//separate from timers
#define T_DIV		64	//16384 Hz

//immediate memory
#define IMM8	(READ_BYTE(_PC-1))
#define IMM16	(READ_WORD(_PC-2))

//memory shortcuts
//only MEM(x) and IO(x) should be used to write to memory
//everything else (vram and oam) should only be used
//for reading conveniently
//ex: instead of reading the tile data at 0x83FD
//read the data at 0x03FD or tile 0x3F,line 0x0D
//oam can be used the same way except with sprite data
//instead of tile data
#define MEM(x)		(gameboy->mem.map[x])
//next two macros return uint16 because it's more convenient in lcd.c
//the last half of T_DATA_0 and the first half of T_DATA_1 overlap
//0x8000 - 0x8FFF
#define T_DATA_0(x)	(READ_WORD(x+0x8000))
//0x8800 - 0x97FF
#define T_DATA_1(x)	(READ_WORD(x+0x8800))
//0x9800 - 0x9BFF
#define T_MAP_0(x)	(gameboy->mem.map[x+0x9800])
//0x9C00 - 0x9FFF
#define T_MAP_1(x)	(gameboy->mem.map[x+0x9C00])
//same as MEM(x) but helps with readability
#define IO(x)		(MEM(x))
//LOW(x) doesn't stop 'overflows' into unused section,
//but it should prevent overflows into io hw register section
#define OAM(x)		(gameboy->mem.map[LOW(x)+_OAM])
#define INTE		(gameboy->mem.map[_IE])

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
#define _IF		0xFF0F
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
#define _IE		0xFFFF

#endif
