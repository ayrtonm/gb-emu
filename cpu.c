#include "cpu.h"
#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

cpu *init_cpu(void)
{
  static cpu *z80;
  z80 = malloc(sizeof(z80));
  _AF = 0x01B0;
  _BC = 0x0013;
  _DE = 0x00D8;
  _HL = 0x014D;
  _SP = 0xFFFE;
  _PC = 0x0100;
  int i;
  for (i = 0; i < 0x80; i++) io[i] = 0;
  write_byte(_LCDC,0x91);
  write_byte(_BGP,0xFC);
  write_byte(_OBP0,0xFF);
  write_byte(_OBP1,0xFF);
  return z80;
}

int emulate(gameboy *X)
{
  uint8 opcode;
  int clk = 0;
  int dt = 0;
  for (;;)
  {
    opcode = read_byte(_PC);
    printf("PC: [0x%x] 0x%x",opcode,_PC);
    getchar();
    if (opcode == 0xCB)
    {
      _PC++;
      opcode = read_byte(_PC);
      _PC += 2;
      dt = cb_cycles[opcode & 0x07];
      switch(opcode)
      {
        #include "cb_opcodes.h"
      }
    }
    else
    {
      _PC += length[opcode];
      dt = cycles[opcode];
      switch(opcode)
      {
        #include "opcodes.h"
      }
    }
    clk += dt;
  }
}
