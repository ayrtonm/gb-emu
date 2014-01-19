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
    //seems to be stuck on line 151 for some roms and never in mode_vram for others
    //need to double check lcd.c
    if (LCD_MODE == MODE_VRAM) {printf("in vram mode, line = %d\n",IO(_LY));getchar();}
    gameboy->div_clk -= dt;
    while(gameboy->div_clk <= 0)
    {
      IO(_DIV)++;
      gameboy->div_clk += T_DIV;
    }
    if (gameboy->lcd.snooze > 0)
    {
      gameboy->lcd.snooze -= dt;
      if (gameboy->lcd.snooze <= 0)
      {
        IO(_IF) |= INT_VBL;
        if (IO(_LCDSTAT & 0x10)) IO(_IF) |= INT_LCD;
      }
    }
    if (IO(_LCDC) & 0x80) step_lcd(dt);
    if (gameboy->timer_on)
    {
      gameboy->timer_clk -= dt;
      while (gameboy->timer_clk <= 0)
      {
        IO(_TIMA)++;
        if (!IO(_TIMA))
        {
          //timer overflow
          IO(_TIMA) = IO(_TMA);
          IO(_IF) |= INT_TIM;
        }
        gameboy->timer_clk += gameboy->timer_period;
      }
    }
    int y,x;
    for (y = 0; y < 144; y++)
    {
      Uint32 *pixels = (Uint32 *)gameboy->lcd.screen->pixels + y*gameboy->lcd.screen->w;
      for (x = 0; x < 160; x++)
      {
        pixels[x] = 
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