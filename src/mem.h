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
  uint8_t b,g,r,a;
} color;

enum keyset {direction, special};
enum keystate {release, press}; 
enum mbc {romonly, mbc1, mbc2, mbc3, mbc4, mbc5, huc3, huc1, mm01};
//if mode is mbc1 then mbcmode can be rom or ram
//if mode is mbc3 then mbcmode can be ram or rtc
enum cartmode {rom, ram, rtc};

const static int tacvals[4] = {256, 4, 16, 64};
const static uint8_t clock_maxvals[5] = {60, 60, 24, 255, 1};

/**
  This class represents the 64 kB memory and external ROM/RAM banks
*/
class mem
{
  public:
    mem(string filename, string memorydump, string savefile);

    uint8_t read_byte_internal(uint16_t address);
    uint8_t read_byte(uint16_t address);
    uint16_t read_word_internal(uint16_t address);
    uint16_t read_word(uint16_t address);
    void write_byte_internal(uint16_t address, uint8_t data);
    void write_byte(uint16_t address, uint8_t data);
    void write_word(uint16_t address, uint16_t data);
    array<color,4> get_palette(uint8_t palette_num);
    void dump_memory();
    bool get_dumpmemory();
    void dump_ram();
    void load_ram();
    void update_keys(keyset k, uint8_t bit, keystate kp);
    uint8_t get_keys(keyset k);
    keyset get_keys_loaded();

    //used in cpu.cpp
    void update_timers(int dt);

  private:
    void load_cart(string filename);
    void print_cart_info();
    //this pointer is set to one of the following handle_x functions during load_cart
    void (mem::*handle_mbc)(uint16_t address, uint8_t data);
    void handle_romonly(uint16_t address, uint8_t data);
    void handle_mbc1(uint16_t address, uint8_t data);
    void handle_mbc2(uint16_t address, uint8_t data);
    void handle_mbc3(uint16_t address, uint8_t data);
    void handle_mbc5(uint16_t address, uint8_t data);
    time_t get_time(void);
    void increment_clock(uint8_t *reg, const uint8_t *maxval);

    void switch_rombanks(int newbank);
    void switch_rambanks(int newbank);
    void update_palette(uint8_t palette, uint8_t value);

    //addressable memory
    array<uint8_t,0x10000> memory = {};
    //pointer to use when reading from 0x4000-0x7fff
    uint8_t *rombank_ptr;
    //pointer to use when reading from 0xa000-0xbfff
    uint8_t *rambank_ptr;
    //extra memory banks
    vector<array<uint8_t,0x4000>> rombanks;
    vector<array<uint8_t,0x2000>> rambanks;
    //real-time clock registers used in MBC3
    //rtc_registers tracks real time
    array<uint8_t,5> rtc_registers;
    //latched_rtc_registers is the values that become latched when writing 0x00 followed by 0x01 to 0x6000-0x7fff
    //these are the registers that are actually readable
    array<uint8_t,5> latched_rtc_registers;
    //index of selected rtc register
    int current_rtc;
    //timer used to increment rtc registers in update_timers()
    long unsigned int rtc_timer;
    mbc mbctype;
    bool rtc_latched;
    //index of selected rombank/rambank
    int current_rombank;
    int current_rambank;
    //number of rombanks/rambanks
    int num_rombanks;
    int num_rambanks;
    bool ramenabled;
    cartmode mbcmode;

    //!First palette is obp0, then obp1, then bgp
    array<color,4> palettes[3];
    bool dumpmemory = false;
    string memorydumpfile, ramdumpfile;

    //need two bytes to store the joypad data since only one is available in addressable memory at any time
    bool loadeddirection;
    uint8_t joydirection;
    uint8_t joyspecial;

    //divtimer counts every 256 CPU clicks
    //making it int to prevent having to cast dt and since it is likely to overflow
    int divtimer;
    int timatimer;
    int tacthreshold;
    int dmatimer;
    bool dmatransfering;
    //sound channel timers
    uint8_t ch1t;
    uint8_t ch2t;
    uint8_t ch3t;
    uint8_t ch4t;
    long int ch1timer;
    long int ch2timer;
    long int ch3timer;
    long int ch4timer;
    struct timespec wait;
};

#endif
