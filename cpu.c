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
  c.IME = 1;
  return c;
}

int emulate(void)
{
  uint8 op;
  int halt = 0;
  int clk = 0;
  int dt = 0;
  SDL_Event event;
  for (;;)
  {
    if (_IME)
    {
      if (INTE & IO(_IF) & INT_VBL) {interrupt(INT_VBL);halt = 0;}
      else if (INTE & IO(_IF) & INT_LCD) {interrupt(INT_LCD);halt = 0;}
      else if (INTE & IO(_IF) & INT_TIM) {interrupt(INT_TIM);halt = 0;}
      else if (INTE & IO(_IF) & INT_SER) {interrupt(INT_SER);halt = 0;}
      else if (INTE & IO(_IF) & INT_JOY) {printf("joypad interrupt started\n");interrupt(INT_JOY);halt = 0;}
    }
    if (halt == 0)
    {
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
        if (printing == 1) printf("0x%x at [%x]\n",op,_PC-length[op]);
        else if (printing == 2)
        {
          system("clear");
          printf("AF: 0x%x    HL: 0x%x\n",_AF,_HL);
          printf("BC: 0x%x    DE: 0x%x\n",_BC,_DE);
          printf("SP: 0x%x    PC: 0x%x\n",_SP,_PC);
          printf("ZF: 0x%x      NF: 0x%x\n",GET(Z_FLAG,_F),GET(N_FLAG,_F));
          printf("HF: 0x%x      CF: 0x%x\n",GET(H_FLAG,_F),GET(C_FLAG,_F));
          printf("IMM16: 0x%x   opcode: 0x%x\n",IMM16,op);
          char a  = getchar();
          if (a == 'q') {return 0;}
        }
        dt = cycles[op];
        switch(op)
        {
          #include "opcodes.h"
        }
      }
    }
    if (halt == 1) dt = 1;
    clk += dt;
    step_lcd(dt);
    gameboy->div_clk -= dt*4;
    if (TIMER_ON) gameboy->time_clk -= dt*4;
    if (gameboy->div_clk <= 0)
    {
      IO(_DIV)++;
      gameboy->div_clk += T_DIV;
    }
    if (gameboy->time_clk <= 0 && TIMER_ON)
    {
      IO(_TIMA)++;
      gameboy->time_clk = gameboy->time_period;
      if (IO(_TIMA) == 0)
      {
        IO(_TIMA) = IO(_TMA);
        REQUEST_INT(INT_TIM);
      }
    }
    while (SDL_PollEvent (&event))
    {
      switch(event.type)
      {
        case SDL_KEYDOWN:
        {
          switch(event.key.keysym.sym)
          {
            case SDLK_LEFT: CLEAR(0x02,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) &= ~0x02;REQUEST_INT(INT_JOY);}break;
            case SDLK_RIGHT: CLEAR(0x01,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) &= ~0x01;REQUEST_INT(INT_JOY);}break;
            case SDLK_DOWN: CLEAR(0x08,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) &= ~0x08;REQUEST_INT(INT_JOY);}break;
            case SDLK_UP: CLEAR(0x04,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) &= ~0x04;REQUEST_INT(INT_JOY);}break;
            case SDLK_z: CLEAR(0x01,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) &= ~0x01;REQUEST_INT(INT_JOY);printf("you pressed z\n");}break;
            case SDLK_x: CLEAR(0x02,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) &= ~0x02;REQUEST_INT(INT_JOY);}break;
            case SDLK_a: CLEAR(0x04,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) &= ~0x04;REQUEST_INT(INT_JOY);}break;
            case SDLK_s: CLEAR(0x08,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) &= ~0x08;REQUEST_INT(INT_JOY);}break;
            case SDLK_q: return 0;
          }
          break;
        }
        case SDL_KEYUP:
        {
          switch(event.key.keysym.sym)
          {
            case SDLK_LEFT: SET(0x02,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) |= 0x02;}break;
            case SDLK_RIGHT: SET(0x01,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) |= 0x01;}break;
            case SDLK_DOWN: SET(0x08,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) |= 0x08;}break;
            case SDLK_UP: SET(0x04,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) |= 0x04;}break;
            case SDLK_z: SET(0x01,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) |= 0x01;}break;
            case SDLK_x: SET(0x02,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) |= 0x02;}break;
            case SDLK_a: SET(0x04,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) |= 0x04;}break;
            case SDLK_s: SET(0x08,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) |= 0x08;}break;
          }
          break;
        }
      }
    }
  }
}

void interrupt(uint8 which)
{
  write_byte(_IF,CLEAR(which,IO(_IF)));
  _IME = 0;
  PUSH(_PCBh,_PCBl);
  _PC = 0x40 + ((int)(log2(which)) << 3);
  printf("0x%x PC = 0x%x\n",which,_PC);
}
