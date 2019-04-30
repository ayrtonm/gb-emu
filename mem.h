#ifndef MEM_H
#define MEM_H
#include "bits.h"
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

const static int tacvals[4] = {1024, 16, 64, 256};

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
    inline uint16 read_word(uint16 address) const {
      return (read_byte(address))+(read_byte(address + 1) << 8);
    };
    //!Adds offset to address if trying to modify ROM Bank N or External RAM
    inline void write_byte(uint16 address, uint8 data) {
      ////prevent writing to OAM while LCD is writing to OAM
      //if ((address < O_UNUSED) && (address >= O_OAM) && (memory[O_IO+IO_LCDSTAT] & 0x02)) {
      //  return;
      //}
      ////prevent writing to OAM and VRAM while LCD is writing to both
      //else if ((((address < O_UNUSED) && (address >= O_OAM)) || ((address < O_ERAM) && (address >= O_VRAM))) && (memory[O_IO+IO_LCDSTAT] & 0x03)) {
      //  return;
      //}
      if (dmatimer != 0) {
        if (address >= O_HRAM) {
          memory[address] = data;
        }
      }
      else {
      memory[address] = data;
      //not totally sure if modifying IO_IR and IO_LCDSTAT is necessary after updating the palettes
      if (address == O_IO+IO_BGP) {
        update_palette(2,memory[O_IO + IO_BGP]);
        //memory[O_IO + IO_IR] &= 0x1f;
        //memory[O_IO + IO_LCDSTAT] &= 0x78;
      }
      else if (address == O_IO+IO_OBP0) {
        update_palette(0,memory[O_IO + IO_OBP0]);
        //memory[O_IO + IO_IR] &= 0x1f;
        //memory[O_IO + IO_LCDSTAT] &= 0x78;
      }
      else if (address == O_IO+IO_OBP1) {
        update_palette(1,memory[O_IO + IO_OBP1]);
        //memory[O_IO + IO_IR] &= 0x1f;
        //memory[O_IO + IO_LCDSTAT] &= 0x78;
      }
      else if (address == O_IO+IO_DIV) {
        memory[O_IO+IO_DIV] = 0x00;
      }
      else if (address == O_IO+IO_TAC) {
        tacthreshold = tacvals[data & 0x03];
      }
      else if (address == O_IO+IO_JOYP) {
        //get lower 4 bits of joydirection/special and combine that with upper 4 bits in memory
        if ((data & JOYP_DIRECTION_SELECTED) == 0x00) {
          memory[O_IO+IO_JOYP] = joydirection + (data & 0xf0);
        }
        else if ((data & JOYP_SPECIAL_SELECTED) == 0x00) {
          memory[O_IO+IO_JOYP] = joyspecial + (data & 0xf0);
        }
        else if ((data & 0xf0) == (JOYP_SPECIAL_SELECTED|JOYP_DIRECTION_SELECTED)) {
          memory[O_IO+IO_JOYP] = joydirection + (data & 0xf0);
        }
      }
      else if (address == O_IO+IO_DMA) {
        //4 MHz * 160 microseconds
        dmatimer = 640;
        if (data <= 0xF1) {
          for (int i = 0; i < 160; i++) {
            memory[0xFE00 + i] = memory[(data << 8) + i];
          }
        }
      }
      }
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
    uint8 joydirection;
    uint8 joyspecial;
};

#endif
