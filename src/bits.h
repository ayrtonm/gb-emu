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
#define REVERSE_BYTES(x) do { \
  x = ((x & 0xF0F0) >> 4) | ((x & 0x0F0F) << 4); \
  x = ((x & 0xCCCC) >> 2) | ((x & 0x3333) << 2); \
  x = ((x & 0xAAAA) >> 1) | ((x & 0x5555) << 1); \
}  while (0)

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef union {
  uint16 w;
  struct {
    #ifdef HIGH_ENDIAN
    uint8 h,l;
    #else
    uint8 l,h;
    #endif
  } b;
} word16;

//the following four tables were originally in cpu.h but they are used in both the interpreted and recompiled versions so they were moved here
//using lookup table to avoid having to do log2
//not totally sure there is a performance improvement but it is easier to implement for now
const static uint16 interrupt_table[16] = {0,0x08,0,0x10,0,0,0,0x18,0,0,0,0,0,0,0,0x20};

const static uint16 length[0x0100] = {
/*0 1 2 3 4 5 6 7 8 9 A B C D E F*/
  1,3,1,1,1,1,2,1,3,1,1,1,1,1,2,1, /*0x*/
  2,3,1,1,1,1,2,1,2,1,1,1,1,1,2,1, /*1x*/
  2,3,1,1,1,1,2,1,2,1,1,1,1,1,2,1, /*2x*/
  2,3,1,1,1,1,2,1,2,1,1,1,1,1,2,1, /*3x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*4x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*5x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*6x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*7x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*8x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*9x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*Ax*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*Bx*/
  1,1,3,3,3,1,2,1,1,1,3,1,3,3,2,1, /*Cx*/
  1,1,3,0,3,1,2,1,1,1,3,0,3,0,2,1, /*Dx*/
  2,1,1,0,0,1,2,1,2,1,3,0,0,0,2,1, /*Ex*/
  2,1,1,1,0,1,2,1,2,1,3,1,0,0,2,1  /*Fx*/
};

const static int cb_cycles[0x08] = {2,2,2,2,2,2,4,2};

const static int cycles[0x0100] = {
/*0 1 2 3 4 5 6 7 8 9 A B C D E F*/
  1,3,2,2,1,1,2,1,5,2,2,2,1,1,2,1, /*0x*/
  1,3,2,2,1,1,2,1,3,2,2,2,1,1,2,1, /*1x*/
  2,3,2,2,1,1,2,1,2,2,2,2,1,1,2,1, /*2x*/
  2,3,2,2,1,1,2,1,2,2,2,2,1,1,2,1, /*3x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*4x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*5x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*6x*/
  2,2,2,2,2,2,1,2,1,1,1,1,1,1,2,1, /*7x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*8x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*9x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*Ax*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*Bx*/
  2,3,3,4,3,4,2,4,2,4,3,1,3,6,2,4, /*Cx*/
  2,3,3,0,3,4,2,4,2,4,3,0,3,0,2,4, /*Dx*/
  3,3,2,0,0,4,2,4,4,1,4,0,0,0,2,4, /*Ex*/
  3,3,2,1,0,4,2,4,3,2,4,1,0,0,2,4  /*Fx*/
};

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

#define LCDSTAT_HBL_INT 0x08
#define LCDSTAT_VBL_INT 0x10
#define LCDSTAT_OAM_INT 0x20
#define LCDSTAT_LYC_INT 0x40

//!oam bit flags
#define OAM_F_BG    0x80
#define OAM_F_YFLIP 0x40
#define OAM_F_XFLIP 0x20
#define OAM_F_PAL   0x10
//!time in each lcd mode in lcd clks
//with these settings opus5.gb shows up on screen but timing is incorrect
//#define T_HBLANK  204
//#define T_VBLANK  4560
//#define T_OAM     80
//#define T_VRAM    172
//with these settings tetris.gb shows up on but I should verify these are correct
#define T_HBLANK  51
#define T_VBLANK  1140
#define T_OAM     20
#define T_VRAM    43
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
#define IO_SB       0x01
#define IO_SC       0x02
#define IO_DIV      0x04
#define IO_TIMA     0x05
#define IO_TMA      0x06
#define IO_TAC	    0x07

//sound registers
#define IO_NR10     0x10
#define IO_NR11     0x11
#define IO_NR12     0x12
#define IO_NR13     0x13
#define IO_NR14     0x14
//0xff15 is unused
#define IO_NR21     0x16
#define IO_NR22     0x17
#define IO_NR23     0x18
#define IO_NR24     0x19
#define IO_NR30     0x1a
#define IO_NR31     0x1b
#define IO_NR32     0x1c
#define IO_NR33     0x1d
#define IO_NR34     0x1e
//0xff1f is unused
#define IO_NR41     0x20
#define IO_NR42     0x21
#define IO_NR43     0x22
#define IO_NR44     0x23
#define IO_NR50     0x24
#define IO_NR51     0x25
#define IO_NR52     0x26

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
