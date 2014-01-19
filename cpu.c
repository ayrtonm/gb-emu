#include "cpu.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

cpu init_cpu(void)
{
  cpu c;
  c.AF.W = 0x01B0;
  c.BC.W = 0x0013;
  c.DE.W = 0x00D8;
  c.HL.W = 0x014D;
  c.SP.W = 0xFFFE;
  c.PC.W = 0x0100;
  return c;
}

int emulate(void)
{
  uint8 op;
  int clk = 0;
  int dt = 0;
  int period;
  for (;;)
  {
    op = READ_BYTE(_PC);
//    printf("PC:[0x%x]=0x%x,AF:0x%x,BC:0x%x,DE:0x%x,HL:0x%x,SP:0x%x\n",_PC,op,_AF,_BC,_DE,_HL,_SP);
    if (op == 0xCB)
    {
      _PC++;
      op = READ_BYTE(_PC);
      _PC += 2;
      dt = cb_cycles[op & 0x07];
      switch(op)
      {
        #include "cb_opcodes.h"
      }
    }
    else
    {
      _PC += length[op];
      dt = cycles[op];
      switch(op)
      {
        #include "opcodes.h"
      }
    }
    clk += dt;
  }
}
