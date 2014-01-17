#include <stdio.h>
#include <stdlib.h>
#include "z80.h"
#include "mem.h"
#include "gpu.h"
#include "input.h"

z80 *init_z80(void)
{
  static z80 *Z;
  Z = calloc(sizeof(Z),1);
  _AF = 0x01B0;
  _BC = 0x0013;
  _DE = 0x00D8;
  _HL = 0x014D;
  _SP = 0xFFFE;
  _PC = 0x0100;
  tima = 0x00;
  tma = 0x00;
  tac = 0x00;
  lcdc = 0x91;
  scy = 0x00;
  scx = 0x00;
  lyc = 0x00;
  bgp = 0xFC;
  obp0 = 0xFF;
  obp1 = 0xFF;
  wy = 0x00;
  wx = 0x00;
  ie = 0x00;
  return Z;
}
int emulate(z80 *Z)
{
  uint8 opcode;
  int clk = 0;
  int dt = 0;
  int run = 1;
  for(;;)
  {
    opcode = read_byte(_PC);
    char string[37];
    snprintf(string,37,"executing opcode 0x%x from [0x%x]\n",opcode,_PC);
    write_log(string);
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
    m_clk = clk*4;
    step_gpu();
    if (check_input() == 1)
      {run = 0;}
    if (run == 0)
      {return 0;}
  }
}