#ifndef COMMON_H
#define COMMON_H

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

//typedefs
typedef unsigned char uint8;
typedef short int uint16;
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

//flags
#define Z_FLAG	0x80
#define N_FLAG	0x40
#define H_FLAG	0x20
#define C_FLAG	0x10
#define CARRY	((GET(C_FLAG,_F)) >> 4)
#define ZERO	((GET(Z_FLAG,_F)) >> 7)

//registers
#define _A	(X->z80->AF.B.h)
#define _F	(X->z80->AF.B.l)
#define _B	(X->z80->BC.B.h)
#define _C	(X->z80->BC.B.l)
#define _D	(X->z80->DE.B.h)
#define _E	(X->z80->DE.B.l)
#define _H	(X->z80->HL.B.h)
#define _L	(X->z80->HL.B.l)
#define _AF	(X->z80->AF.W)
#define _BC	(X->z80->BC.W)
#define _DE	(X->z80->DE.W)
#define _HL	(X->z80->HL.W)
#define _SP	(X->z80->SP.W)
#define _PC	(X->z80->PC.W)
#define _PCBl	(X->z80->PC.B.l)
#define _PCBh	(X->z80->PC.B.h)
#define _IME	(X->z80->IME)

//memory access
#define IMM8	(read_byte(_PC-1))
#define IMM16	(read_word(_PC-2))

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
