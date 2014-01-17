#ifndef MEM_H
#define MEM_H

/*memory offsets*/
#define _ROM		0x0000
#define _BANK		0x4000
#define _VRAM		0x8000
#define _ERAM		0xA000
#define _WRAM		0xC000
#define _ECHO		0xE000
#define _OAM		0xFE00
#define	_HRAM		0xFF80

/*absolute addresses*/
#define _JOYP		0xFF00
#define _DIV		0xFF04/*variable is divr not div*/
#define _TIMA		0xFF05
#define _TMA		0xFF06
#define _TAC		0xFF07
#define _LCDC		0xFF40
#define _LCDSTAT	0xFF41
#define _SCY		0xFF42
#define _SCX		0xFF43
#define _LY		0xFF44/*variable is line not ly*/
#define _LYC		0xFF45
#define _DMA		0xFF46
#define _BGP		0xFF47
#define _OBP0		0xFF48
#define _OBP1		0xFF49
#define _WY		0xFF4A
#define _WX		0xFF4B
#define _IF		0xFF0F/*variable is iff not if*/
#define _IE		0xFFFF

typedef unsigned char uint8;
typedef short int uint16;

uint8 mem[0x010000];
uint8 *cart;
uint8 mode;//gpu variable
uint8 divr,tima,tma,tac,lcdc,lcdstat,scy,scx,line,lyc,bgp,obp0,obp1,wy,wx,iff,ie;

/*rom and ram sizes are shifted 1 byte >>*/
static uint16 rom_sizes[8] = {0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000};
static uint8 ram_sizes[4] = {0x00,0x08,0x20,0x80};

extern uint8 *load_cart(char *);
extern int print_header(char *);
extern uint8 read_byte(uint16);
extern uint16 read_word(uint16);
extern void write_byte(uint16, uint8);
extern void write_word(uint16, uint16);

#endif
