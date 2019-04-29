#ifndef BITS_H
#define BITS_H
#define BIT(x)      (1 << (x))
#define GET(a,b)    ((a) & (b))
#define SET(a,b)    ((b) |= (a))
#define CLEAR(a,b)    ((b) &= ~(a))
#define TOGGLE(a,b)    ((b) ^= (a))
#define SWAP(x)		(((x) & 0xF0) >> 4 | ((x) & 0x0F) << 4)
//#define SWITCH(a,b)    (a == b ? : TGL(a,b);TGL(b,a);TGL(a,b))
//return last bit of x
#define CHECKNEG(x) (GET(sizeof(x)-1,x) >> sizeof(x)-1)
#define LOW(x)     ((x) & 0xff)
#define HIGH(x)    ((x) >> 8)
#define MAX(a,b)    ((a) > (b) ? (a) : (b))
#define MIN(a,b)    ((a) > (b) ? (b) : (a))
#define REVERSE_WORD(x) do { \
  x = (x & 0xFF00) >> 8 | (x & 0x00FF) << 8; \
  x = (x & 0xF0F0) >> 4 | (x & 0x0F0F) << 4; \
  x = (x & 0xCCCC) >> 2 | (x & 0x3333) << 2; \
  x = (x & 0xAAAA) >> 1 | (x & 0x5555) << 1; \
}  while (0)

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef union
{
  uint16 w;
  struct
  {
    #ifdef HIGH_ENDIAN
    uint8 h,l;
    #else
    uint8 l,h;
    #endif
  } b;
} word16;

//!interrupts
#define INT_VBL   0x01
#define INT_LCD   0x02
#define INT_TIM   0x04
#define INT_SER   0x08
#define INT_JOY   0x10
//!lcdc bit flags used in lcd.c
#define LCDC_ENABLE     0x80
#define LCDC_WIN_MAP    0x40
#define LCDC_WIN_ENABLE 0x20
#define LCDC_BG_DATA    0x10
#define LCDC_BG_MAP     0x08
#define LCDC_OBJ_SIZE   0x04
#define LCDC_OBJ_ENABLE 0x02
#define LCDC_BG_ENABLE  0x01
//!oam bit flags
#define OAM_F_BG    0x80
#define OAM_F_YFLIP 0x40
#define OAM_F_XFLIP 0x20
#define OAM_F_PAL   0x10
//!time in each lcd mode in lcd clks
#define T_HBLANK  204
#define T_VBLANK  4560
#define T_OAM     80
#define T_VRAM    172
#define TIO_LY_INC  ((T_HBLANK)+(T_OAM)+(T_VRAM))
//timer flags
#define TIMER_ENABLED 0x04
//joypad flags
#define JOYP_DIRECTION_SELECTED 0x10
#define JOYP_SPECIAL_SELECTED   0x20
#define JOYP_A_BIT 0x01
#define JOYP_B_BIT 0x02
#define JOYP_SELECT_BIT 0x04
#define JOYP_START_BIT 0x08
#define JOYP_R_BIT 0x01
#define JOYP_L_BIT 0x02
#define JOYP_U_BIT 0x04
#define JOYP_D_BIT 0x08
//!cpu flags
#define F_Z       0x80
#define F_N       0x40
#define F_H       0x20
#define F_C       0x10
//!offsets for memory sections
#define O_ROMBN    0x4000 
#define O_VRAM     0x8000
#define O_ERAM     0xa000
#define O_WRAM0    0xc000
#define O_WRAM1    0xd000
#define O_ECHO0    0xe000
#define O_ECHO1    0xf000
#define O_OAM      0xfe00
#define O_UNUSED   0xfea0
#define O_IO       0xff00
#define O_HRAM     0xff80
#define O_HRAM_END 0xfffe
#define O_IE       0xffff
//!offsets within vram
#define V_TD_0    0x0000
#define V_TD_1    0x0800
#define V_MD_0    0x1800
#define V_MD_1    0x1c00
//!offsets for I/O ports
#define IO_JOYP     0x00
#define IO_DIV      0x04
#define IO_TIMA     0x05
#define IO_TMA      0x06
#define IO_TAC	    0x07
#define IO_IR	      0x0F
#define IO_LCDC		  0x40
#define IO_LCDSTAT  0x41
#define IO_SCY      0x42
#define IO_SCX      0x43
#define IO_LY       0x44
#define IO_LYC      0x45
#define IO_DMA      0x46
#define IO_BGP      0x47
#define IO_OBP0     0x48
#define IO_OBP1     0x49
#define IO_WY       0x4A
#define IO_WX       0x4B
#endif
