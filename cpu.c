#include "cpu.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>//for log2 function
//#define BREAK 100000
#undef BREAK

cpu init_cpu(void)
{
  cpu c;
  c.AF.W = 0x01B0;
  c.BC.W = 0x0013;
  c.DE.W = 0x00D8;
  c.HL.W = 0x014D;
  c.SP.W = 0xFFFE;
  c.PC.W = 0x0100;
  c.IME = 0;
  c.ei_delay = false;
  c.halt = false;
  return c;
}

int emulate(void)
{
  //j is used for counting executed opcodes for debugging
  int j = 1;
  uint8 op;
  int clk = 0;
  int dt = 0;
  SDL_Event event;
  for (;;)
  {
    if (_HALT || _IME)
    {
      int i = 1;
      while ((!(IO(_IR) & i) || !(IO(_IE) & i)) && (i <= 0x10))
      {
        i <<= 1;
      }
      if (i <= 0x10)
      {
        if (_HALT) {_PC++;}
        if (_IME)
        {
          _IME = 0;
          write_byte(_IR,CLEAR(i,IO(_IR)));
          EI_DELAY = false;
          PUSH(_PCBh,_PCBl);
          _PC = 0x40 + ((int)(log2(i)) << 3);
        }
      }
    }
    if (_HALT) dt = 1;
    if (EI_DELAY) {_IME = 1;EI_DELAY = false;}
    if (!_HALT)
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
        j++;
      }
      else
      {
        _PC += length[op];
        if (printing == opcodes) printf("0x%x\n",op,_PC-length[op]);
        else if (printing == debug)
        {
#ifdef BREAK
          if (j >= BREAK) {
#endif
          system("clear");
          printf("AF: 0x%x      \nHL: 0x%x\n",_AF,_HL);
          printf("BC: 0x%x      \nDE: 0x%x\n",_BC,_DE);
          printf("SP: 0x%x      \nPC: 0x%x\n",_SP,_PC-length[op]);
          printf("ZF: 0x%x      \nNF: 0x%x\n",GET(Z_FLAG,_F),GET(N_FLAG,_F));
          printf("HF: 0x%x      \nCF: 0x%x\n",GET(H_FLAG,_F),GET(C_FLAG,_F));
          printf("opcode: 0x%x  \n",op);
          printf("IMM8: 0x%x    \nIMM16: 0x%x\n",IMM8,IMM16);
          printf("IME: %d       \nn: %d\n",_IME,j);
          printf("IR: %x\nIE: %x\n",IO(_IR),IO(_IE));
          printf("JOYP: %x\n",IO(_JOYP));
          printf("[  SP  ] =  0x%x\n",MEM(_SP));
          printf("[ SP-1 ] =  0x%x\n",MEM(_SP-1));
          printf("[ SP-2 ] =  0x%x\n",MEM(_SP-2));
          printf("[ SP-3 ] =  0x%x\n",MEM(_SP-3));
          printf("[ SP-4 ] =  0x%x\n",MEM(_SP-4));
          printf("[ SP-5 ] =  0x%x\n",MEM(_SP-5));
          printf("[ SP-6 ] =  0x%x\n",MEM(_SP-6));
          printf("[ SP-7 ] =  0x%x\n",MEM(_SP-7));
          printf("[ SP-8 ] =  0x%x\n",MEM(_SP-8));
          printf("[%x][%x][%x][%x][%x][%x][%x]\n",MEM(_PC-length[op]-3),MEM(_PC-length[op]-2),MEM(_PC-length[op]-1),MEM(_PC-length[op]),MEM(_PC-length[op]+1),MEM(_PC-length[op]+2),MEM(_PC-length[op]+3));
          char a  = getchar();
          if (a == 'q') {return 0;}
#ifdef BREAK
}
#endif
        }
        dt = cycles[op];
        switch(op)
        {
          #include "opcodes.h"
        }
        j++;
      }
    }
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
            case SDLK_LEFT: gameboy->key_bitmap |= LEFT_K;if (!(IO(_JOYP) & 0x10)) {IO(_JOYP) &= ~0x02;REQUEST_INT(INT_JOY);}break;
            case SDLK_RIGHT: gameboy->key_bitmap |= RIGHT_K;if (!(IO(_JOYP) & 0x10)) {IO(_JOYP) &= ~0x01;REQUEST_INT(INT_JOY);}break;
            case SDLK_DOWN: gameboy->key_bitmap |= DOWN_K;if (!(IO(_JOYP) & 0x10)) {IO(_JOYP) &= ~0x08;REQUEST_INT(INT_JOY);}break;
            case SDLK_UP: gameboy->key_bitmap |= UP_K;if (!(IO(_JOYP) & 0x10)) {IO(_JOYP) &= ~0x04;REQUEST_INT(INT_JOY);}break;
            case SDLK_z: gameboy->key_bitmap |= A_K;if (!(IO(_JOYP) & 0x20)) {IO(_JOYP) &= ~0x01;REQUEST_INT(INT_JOY);}break;
            case SDLK_x: gameboy->key_bitmap |= B_K;if (!(IO(_JOYP) & 0x20)) {IO(_JOYP) &= ~0x02;REQUEST_INT(INT_JOY);}break;
            case SDLK_a: gameboy->key_bitmap |= STA_K;if (!(IO(_JOYP) & 0x20)) {IO(_JOYP) &= ~0x08;REQUEST_INT(INT_JOY);}break;
            case SDLK_s: gameboy->key_bitmap |= SEL_K;if (!(IO(_JOYP) & 0x20)) {IO(_JOYP) &= ~0x04;REQUEST_INT(INT_JOY);}break;
            case SDLK_q: return 0;
          }
          break;
        }
        case SDL_KEYUP:
        {
          switch(event.key.keysym.sym)
          {
            case SDLK_LEFT: gameboy->key_bitmap &= ~LEFT_K;if (!(IO(_JOYP) & 0x10)) {IO(_JOYP) |= 0x02;}break;
            case SDLK_RIGHT: gameboy->key_bitmap &= ~RIGHT_K;if (!(IO(_JOYP) & 0x10)) {IO(_JOYP) |= 0x01;}break;
            case SDLK_DOWN: gameboy->key_bitmap &= ~DOWN_K;if (!(IO(_JOYP) & 0x10)) {IO(_JOYP) |= 0x08;}break;
            case SDLK_UP: gameboy->key_bitmap &= ~UP_K;if (!(IO(_JOYP) & 0x10)) {IO(_JOYP) |= 0x04;}break;
            case SDLK_z: gameboy->key_bitmap &= ~A_K;if (!(IO(_JOYP) & 0x20)) {IO(_JOYP) |= 0x01;}break;
            case SDLK_x: gameboy->key_bitmap &= ~B_K;if (!(IO(_JOYP) & 0x20)) {IO(_JOYP) |= 0x02;}break;
            case SDLK_a: gameboy->key_bitmap &= ~STA_K;if (!(IO(_JOYP) & 0x20)) {IO(_JOYP) |= 0x08;}break;
            case SDLK_s: gameboy->key_bitmap &= ~SEL_K;if (!(IO(_JOYP) & 0x20)) {IO(_JOYP) |= 0x04;}break;
          }
          break;
        }
      }
    }
  }
}
