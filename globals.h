#ifndef GLOBALS_H
#define GLOBALS_H
#include "cpu.h"
#include "mem.h"
#include "common.h"

typedef struct gb
{
  cpu *cpu;
  mbc *mbc;
  mem *mem;
} gb;

gb *gameboy;

extern int emulate(gb *);

#endif
