#include "cpu.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>//for log2 function
#define DEBUG
#ifdef DEBUG
#include <ncurses.h>//for debugging
#endif

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
#ifdef DEBUG
  initscr();
  raw();
  keypad(stdscr,TRUE);
  noecho();
  char c;
  int i = -1;
  int j = -1;
  char *modes[4] = {"HBLANK","VBLANK","OAM","VRAM"};
#endif

  uint8 op;
  int clk = 0;
  int dt = 0;
  SDL_Event event;
  for (;;)
  {
#ifdef DEBUG
  usleep(10000);
  clear();
  mvprintw(0,0,"AF = 0x%x",_AF);
  mvprintw(1,0,"BC = 0x%x",_BC);
  mvprintw(2,0,"DE = 0x%x",_DE);
  mvprintw(3,0,"HL = 0x%x",_HL);
  mvprintw(4,0,"SP = 0x%x",_SP);
  mvprintw(5,0,"PC = 0x%x",_PC);
  mvprintw(6,0,"LCDC = 0x%x",IO(_LCDC));
  mvprintw(7,0,"LCDSTAT = 0x%x MODE = %s",IO(_LCDSTAT),modes[LCD_MODE]);
  mvprintw(8,0,"LY = %d",IO(_LY));
  mvprintw(9,0,"NEXT OPCODE = 0x%x",READ_BYTE(_PC));
  mvprintw(10,0,"STACK = 0x%x",MEM(_SP));
  mvprintw(11,0,"IF = 0x%x",MEM(_IF));
  mvprintw(12,0,"IE = 0x%x",MEM(_IE));
  mvprintw(13,0,"LCD CLK = %d",gameboy->lcd.clk);
  mvprintw(14,0,"DIV CLK = %d",gameboy->div_clk);
  mvprintw(15,0,"TIMER CLK = %d",gameboy->time_clk);
  mvprintw(16,0,"TIMER PERIOD = %d",gameboy->time_period);
  mvprintw(17,0,"CLK = %d",clk);
  mvprintw(18,0,"JOYP = 0x%x",IO(_JOYP));
  mvprintw(19,0,"IMM8 = 0x%x",IMM8);
  mvprintw(20,0,"IMM16 = 0x%x",IMM16);
  refresh();
  if (i == -1 && j == -1) c = getch();
  if (i == IO(_LY)) {c = 'n';}
  else {i = -1;}
  if (j == 0)
  {
    if (IO(_LY) == 155) {j = -1;}
    else {c == 'n';}
  }
  if (c == 'q') {endwin();return 0;}
  if (c == 'l') {i = IO(_LY);c = 'n';}
  if (c == 'f') {j = 0;c = 'n';}
  if (c == 'n') {
#endif
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
            case SDLK_LEFT: CLEAR(0x02,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) &= ~0x02;}break;
            case SDLK_RIGHT: CLEAR(0x01,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) &= ~0x01;}break;
            case SDLK_DOWN: CLEAR(0x08,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) &= ~0x08;}break;
            case SDLK_UP: CLEAR(0x04,gameboy->joyp[0]);if (IO(_JOYP) & 0x10) {IO(_JOYP) &= ~0x04;}break;
            case SDLK_z: CLEAR(0x01,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) &= ~0x01;}break;
            case SDLK_x: CLEAR(0x02,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) &= ~0x02;}break;
            case SDLK_a: CLEAR(0x04,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) &= ~0x04;}break;
            case SDLK_s: CLEAR(0x08,gameboy->joyp[1]);if (IO(_JOYP) & 0x20) {IO(_JOYP) &= ~0x08;}break;
          }
          REQUEST_INT(INT_JOY);
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
#ifdef DEBUG
  }
#endif
}

void interrupt(uint8 which)
{
  write_byte(_IF,CLEAR(which,IO(_IF)));
  _IME = 0;
  PUSH(_PCBh,_PCBl);
  _PC = 0x40 + ((int)(log2(which)) << 3);
}
