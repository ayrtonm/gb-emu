#include "cpu.h"
/**
  makes cpu struct and initializes its registers to values when gameboy is reset
  ei_delay and halt aren't actual hw registers, halt is used to see if halt or stop was the last opcode executed and ei_delay is used to delay interrupts one opcode
**/
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
  c.ei_delay = 0;
  c.halt = 0;
  return c;
}
/**
  this function reads the opcode at the address specified by program counter(_PC), runs opcode, increments clock, updates lcd depending on how much time passed(dt)
  still need to test timers and div register
  joypad seems to be working normally assuming glitches in racing game and spaceship game are due to small problems with opcodes and setting flags which is very likely
  everything inside long comment lines will eventually be deleted after all opcodes are debugged and working properly
**/
int emulate(void)
{
  uint8 op;
  //lcdstat is initialized to 0x91 so lcd mode is initially VBLANK(0x01)
  gameboy->lcd.clk = T_VBLANK;
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
          EI_DELAY = 0;
          PUSH(_PCBh,_PCBl);
          _PC = 0x40 + interrupt_table[i-1];
        }
      }
    }
    if (_HALT) {dt = 1*4;}
    if (EI_DELAY) {_IME = 1;EI_DELAY = 0;}
    if (!_HALT)
    {
      op = READ_BYTE(_PC);
      if (op == 0xCB)
      {
        _PC++;
        op = READ_BYTE(_PC);
        _PC ++;
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
    }
    //gameboy->lcd.clk += dt;
    step_lcd(dt);
    gameboy->div_clk -= dt;
    if (TIMER_ON) gameboy->time_clk -= dt;
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
        case SDL_VIDEORESIZE:
        {
          if (event.resize.w != 0 && event.resize.h != 0)
          {
            gameboy->lcd.visible = SDL_SetVideoMode(MAX(160,event.resize.w),MAX(144,event.resize.h),32,SDL_HWSURFACE|SDL_RESIZABLE);
            gameboy->lcd.scale = MIN((float)gameboy->lcd.visible->w/160.0,(float)gameboy->lcd.visible->h/144.0);
            gameboy->lcd.offset.x = (gameboy->lcd.visible->w - (160*gameboy->lcd.scale))/2;
            gameboy->lcd.offset.y = (gameboy->lcd.visible->h - (144*gameboy->lcd.scale))/2;
            SDL_BlitSurface(zoomSurface(gameboy->lcd.screen,gameboy->lcd.scale,gameboy->lcd.scale,0),NULL,gameboy->lcd.visible,&gameboy->lcd.offset);
          }
        }
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
            case SDLK_q: free(gameboy);return 0;
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
