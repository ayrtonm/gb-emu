#ifndef MEM_H
#define MEM_H
#include "bits.h"
#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

typedef struct color {
  uint8 a,r,g,b;
} color;

enum keys {direction, special};
enum keypress {up, down}; 
enum mbc {romonly, mbc1, mbc2, mbc3, mbc4, mbc5, huc3, huc1, mm01};
//if mode is mbc1 then mbcmode can be rom or ram
//if mode is mbc3 then mbcmode can be ram or rtc
enum cartmode {rom, ram, rtc};

const static int tacvals[4] = {256, 4, 16, 64};

class mem
{
  public:
    mem(string filename, string memorydump = "");
    inline uint8 read_byte(uint16 address) const {
      if ((address < O_VRAM) && (address >= 0x4000)) {
        return (*rombank_ptr)[address];
      }
      else if ((address < O_WRAM0) && (address >= O_ERAM)) {
        return (*rambank_ptr)[address];
      }
      else {
        return memory[address];
      }
    };
    inline uint16 read_word(uint16 address) const {
      return (read_byte(address))+(read_byte(address + 1) << 8);
    };

    void write_byte_internal(uint16 address, uint8 data);
    void write_byte(uint16 address, uint8 data);
    inline void write_word(uint16 address, uint16 data)
    {
      write_byte(address,data & 0x00ff);
      write_byte(address+1,data >> 8);
    };

    //used in lcd.cpp
    array<color,4> get_palette(uint8 palette_num);

    //used in keypad.cpp
    void dump_memory();
    bool get_dumpmemory() {
      return dumpmemory; 
    }
    void update_keys(keys k, uint8 bit, keypress kp);
    uint8 get_keys(keys k);
    keys get_keys_loaded();

    //used in cpu.cpp
    void update_timers(int dt);

  private:
    void load_cart(string filename);
    //this pointer is set to one of the following handle_x functions during load_cart
    void (mem::*handle_mbc)(uint16 address, uint8 data);
    void handle_romonly(uint16 address, uint8 data);
    void handle_mbc1(uint16 address, uint8 data);
    void handle_mbc2(uint16 address, uint8 data);
    void handle_mbc3(uint16 address, uint8 data);

    void switch_rombanks(int newbank);
    void switch_rambanks(int newbank);
    void update_palette(uint8 palette, uint8 value);

    //addressable memory
    array<uint8,0x10000> memory = {};
    array<uint8,0x4000> *rombank_ptr;
    array<uint8,0x2000> *rambank_ptr;
    vector<array<uint8,0x4000>> rombanks;
    vector<array<uint8,0x2000>> rambanks;
    array<uint8,5> rtc_registers;
    int current_rtc;
    mbc mbctype;
    int current_rombank;
    int current_rambank;
    int num_rombanks;
    int num_rambanks;
    bool ramenabled;
    cartmode mbcmode;

    //!First palette is obp0, then obp1, then bgp
    array<color,4> palettes[3];
    bool dumpmemory = false;
    string memorydumpfile;

    //need two bytes to store the joypad data since only one is available in addressable memory at any time
    bool loadeddirection;
    uint8 joydirection;
    uint8 joyspecial;

    //divtimer counts every 256 CPU clicks
    //making it int to prevent having to cast dt and since it is likely to overflow
    int divtimer;
    int timatimer;
    int tacthreshold;
    int dmatimer;
    bool dmatransfering;
    //sound channel timers
    uint8 ch1t;
    uint8 ch2t;
    uint8 ch3t;
    uint8 ch4t;
    long int ch1timer;
    long int ch2timer;
    long int ch3timer;
    long int ch4timer;
};

#endif
