#ifndef GLOBALS_H
#define GLOBALS_H
#include "cpu.h"
#include "mem.h"
#include "common.h"

typedef struct gameboy
{
  //main variables
  cpu *cpu;
  mbc *cart;
  uint8 mem[0x010000];

  uint8 tile_data_0[0x0C00];
  uint8 tile_data_1[0x0C00];
  uint8 tile_map_0[0x0400];
  uint8 tile_map_1[0x0400];
  uint8 oam[0xA0];
  uint8 io[0x80];

} gameboy;

#endif