#ifndef GLOBALS_H
#define GLOBALS_H
#include <vector>
#include <string>
#include <array>
#include <SDL/SDL.h>
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


using namespace std;

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

class mem 
{
  public:
    mem(string filename);
    //!ROM Bank 0 stores the first 16 kB of the cartridge
    array<uint8,0x4000> romb0;
    /*!ROM Bank N stores the rest of the cartridge. The cpu is only allowed to access one 16 kB bank at a time. The accessible sections are based on MBC settings.
      @c load_cart() takes care of resizing this array to fit the entire cartridge
      @c read_byte() takes care of adding offset to address to access correct bank
      */
    vector<uint8> rombn;
    array<uint8,0x2000> vram;
    //!External RAM has switchable 8 kB banks. Bank switching is handled the same way as @c rombn
    vector<uint8> eram;
    array<uint8,0x1000> wramb0;
    array<uint8,0x1000> wramb1;
    array<uint8,0xA0> oam;
    array<uint8,0x80> io;
    array<uint8,0x7f> hram;
//    vector<uint8> hram;
    uint8 interrupt_enable;
    const SDL_PixelFormat *format;
    array<Uint32,4> pal_bgp;
    array<Uint32,4> pal_obp0;
    array<Uint32,4> pal_obp1;
    //!Sets @c rombn and @c eram sizes
    void load_cart(string filename);
    //!Adds offset to address if trying to access ROM Bank N or External RAM
    uint8 read_byte(uint16 address) const;
    uint16 read_word(uint16 address) const;
    //!Adds offset to address if trying to modify ROM Bank N or External RAM
    void write_byte(uint16 address, uint8 data);
    void write_word(uint16 address, uint16 data);
    //!called every time @c write_byte is called with an address between 0xff00 and 0xff80
    void update_io(void);
    void update_palette(uint8 palette, uint8 value);
};

class lcd
{
  public:
    lcd();
    ~lcd();
    SDL_Surface *screen, *visible;
    SDL_Rect offset;
    SDL_Event event;
    float scale;
    array<uint8,160> linebuffer;
    int clk;
    void step_lcd(uint8 dt, mem &m);
    void compareLYtoLYC(mem &m);
    void draw_line(mem &m);

    uint16 reverse_word(uint16 input);
    void draw_sprites(mem &m);
    int parse_events(void);
    SDL_Surface *zoomSurface (SDL_Surface * src, double zoomx, double zoomy, int smooth);
};

class cpu 
{
  public:
    cpu();
    word16 af, bc, de, hl, sp, pc;
    uint8 ime;
    uint8 ei_delay;
    uint8 halt;
    int emulate(mem &m, lcd &l);
    void print_registers(void);
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
//!cpu flags
#define F_Z       0x80
#define F_N       0x40
#define F_H       0x20
#define F_C       0x10
//!offsets for memory sections
#define O_ROMBN    0x4000 
#define O_VRAM   0x8000
#define O_ERAM     0xa000
#define O_WRAM0    0xc000
#define O_WRAM1    0xd000
#define O_ECHO0    0xe000
#define O_ECHO1    0xf000
#define O_OAM    0xfe00
#define O_UNUSED   0xfea0
#define O_IO       0xff00
#define O_HRAM     0xff80
#define O_HRAM_END 0xfffe
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
