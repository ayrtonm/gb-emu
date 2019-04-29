#ifndef MEM_H
#define MEM_H
#include "bits.h"
#include <vector>
#include <array>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

typedef struct color {
  uint8 a,r,g,b;
} color;

class mem 
{
  public:
    mem(string filename, string memorydump = "");
    //!Sets @c rombn and @c eram sizes
    void load_cart(string filename);
    //!Adds offset to address if trying to access ROM Bank N or External RAM
    inline uint8 read_byte(uint16 address) const {
      return memory.at(address);
    };
    //uint8 read_byte_slow(uint16 address) const;
    inline uint16 read_word(uint16 address) const {
      return (read_byte(address))+(read_byte(address + 1) << 8);
    };
    //!Adds offset to address if trying to modify ROM Bank N or External RAM
    inline void write_byte(uint16 address, uint8 data) {
      memory.at(address) = data;
      if ((address == O_IO+IO_BGP) || (address == O_IO+IO_OBP0) || (address == O_IO+IO_OBP1)) {
        update_io();
      };
    };
    //void write_byte_slow(uint16 address, uint8 data);
    inline void write_word(uint16 address, uint16 data)
    {
      write_byte(address,data & 0x00ff);
      write_byte(address+1,data >> 8);
    };
    //!called every time @c write_byte is called with an address between 0xff00 and 0xff80
    void update_io(void);
    void update_palette(uint8 palette, uint8 value);
    array<color,4> get_palette(uint8 palette_num);
    void set_format(const SDL_PixelFormat *fmt);
    void dump_memory();
    bool check_memory_dump() {
      return dumpmemory; 
    }
  private:
    //addressable memory
    array<uint8,0x10000> memory;
    //!ROM Bank 0 stores the first 16 kB of the cartridge
    //array<uint8,0x4000> romb0;
    /*!ROM Bank N stores the rest of the cartridge. The cpu is only allowed to access one 16 kB bank at a time. The accessible sections are based on MBC settings.
      @c load_cart() takes care of resizing this array to fit the entire cartridge
      @c read_byte() takes care of adding offset to address to access correct bank
      */
    //vector<uint8> rombn;
    //array<uint8,0x2000> vram;
    //!External RAM has switchable 8 kB banks. Bank switching is handled the same way as @c rombn
    //temporarily making this a fixed 8 kB bank initiallized to 0 to make dumping memory easier
    //vector<uint8> eram;
    //array<uint8,0x2000> eram;
    //array<uint8,0x1000> wramb0;
    //array<uint8,0x1000> wramb1;
    //array<uint8,0xA0> oam;
    //array<uint8,0x80> io;
    //array<uint8,0x7f> hram;
//    vector<uint8> hram;
    //uint8 interrupt_enable;
    const SDL_PixelFormat *format;
    //!First palette is obp0, then obp1, then bgp
    array<color,4> palettes[3];
    bool dumpmemory = false;
    string memorydumpfile;
};

#endif
