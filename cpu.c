#include "cpu.h"
#include "mem.h"
#include "globals.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

cpu *init_cpu(void)
{
  cpu *z80;
  z80 = malloc(sizeof(cpu));
  z80->AF.W = 0x01B0;
  z80->BC.W = 0x0013;
  z80->DE.W = 0x00D8;
  z80->HL.W = 0x014D;
  z80->SP.W = 0xFFFE;
  z80->PC.W = 0x0100;
  return z80;
}

int emulate(gb *gameboy)
{
  uint8 opcode;
  int clk = 0;
  int dt = 0;
  for (;;)
  {
    opcode = READ_BYTE(_PC);
    printf("PC: [0x%x] 0x%x",opcode,_PC);
    getchar();
    if (opcode == 0xCB)
    {
      _PC++;
      opcode = READ_BYTE(_PC);
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
