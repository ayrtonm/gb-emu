#include "cpu.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>//for log2 function

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
  int i = 0;
  for (;;)
  {
    if (_IME)
    {
      if (INTE & IO(_IF) & INT_VBL) interrupt(INT_VBL);
      else if (INTE & IO(_IF) & INT_LCD) interrupt(INT_LCD);
      else if (INTE & IO(_IF) & INT_TIM) interrupt(INT_TIM);
      else if (INTE & IO(_IF) & INT_SER) interrupt(INT_SER);
      else if (INTE & IO(_IF) & INT_JOY) interrupt(INT_JOY);
    }
    op = READ_BYTE(_PC);
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
    step_lcd(dt);
    gameboy->div_clk -= dt*4;
    gameboy->time_clk -= dt*4;
    if (gameboy->div_clk <= 0)
    {
      IO(_DIV)++;
    }
    if (gameboy->time_clk <= 0 && TIMER_ON)
    {
      IO(_TIMA)++;
      if (IO(_TIMA) == 0)
      {
        IO(_TIMA) = IO(_TMA);
        write_byte(_IF,SET(INT_TIM,IO(_IF)));//request interrupt
      }
    }
  }
}

void interrupt(uint8 which)
{
  write_byte(_IF,CLEAR(which,IO(_IF)));
  PUSH(_PCBh,_PCBl);
  _PC = 0x40 + ((int)(log2(which)) << 3);
}