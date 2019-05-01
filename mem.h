#ifndef MEM_H
#define MEM_H
#include "bits.h"
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

const static int tacvals[4] = {256, 4, 16, 64};

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
    void write_byte(uint16 address, uint8 data);
    inline uint16 read_word(uint16 address) const {
      return (read_byte(address))+(read_byte(address + 1) << 8);
    };
    inline void write_word(uint16 address, uint16 data)
    {
      write_byte(address,data & 0x00ff);
      write_byte(address+1,data >> 8);
    };
    void update_palette(uint8 palette, uint8 value);
    array<color,4> get_palette(uint8 palette_num);
    void dump_memory();
    bool check_memory_dump() {
      return dumpmemory; 
    }
    void update_timers(int dt);
    void update_keys(bool special, uint8 bit, bool down);
    uint8 get_keys(bool special);
    bool direction_loaded();
  private:
    //addressable memory
    array<uint8,0x10000> memory;
    //!First palette is obp0, then obp1, then bgp
    array<color,4> palettes[3];
    bool dumpmemory = false;
    string memorydumpfile;
    //divtimer counts every 256 CPU clicks
    //making it int to prevent having to cast dt and since it is likely to overflow
    int divtimer;
    int timatimer;
    int tacthreshold;
    int dmatimer;
    //need two bytes to store the joypad data since only one is available in addressable memory at any time
    bool loadeddirection;
    uint8 joydirection;
    uint8 joyspecial;
};

#endif
