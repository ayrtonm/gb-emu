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
      return memory[address];
    };
    //uint8 read_byte_slow(uint16 address) const;
    inline uint16 read_word(uint16 address) const {
      return (read_byte(address))+(read_byte(address + 1) << 8);
    };
    //!Adds offset to address if trying to modify ROM Bank N or External RAM
    inline void write_byte(uint16 address, uint8 data) {
      memory[address] = data;
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
    void dump_memory();
    bool check_memory_dump() {
      return dumpmemory; 
    }
  private:
    //addressable memory
    array<uint8,0x10000> memory;
    //!First palette is obp0, then obp1, then bgp
    array<color,4> palettes[3];
    bool dumpmemory = false;
    string memorydumpfile;
};

#endif
