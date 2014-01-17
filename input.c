#include <SDL/SDL.h>
#include "input.h"
#include "z80.h"

void init_input(void)
{
  //start,select,B,A
  rows[0] = 0x0F;

  //Down,Up,Left,Right
  rows[1] = 0x0F;

  //bits 4 and 5 of JOYP
  column = 0;
}

int check_input(void)
{
  SDL_Event event;
  while (SDL_PollEvent (&event))
  {
    switch(event.type)
    {
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_LEFT:  CLEAR(0x02,rows[1]);write_log("  you pressed down left\n");break;
          case SDLK_RIGHT:  CLEAR(0x01,rows[1]);write_log("  you pressed down right\n");break;
          case SDLK_DOWN:  CLEAR(0x08,rows[1]);write_log("  you pressed down down\n");break;
          case SDLK_UP:  CLEAR(0x04,rows[1]);write_log("  you pressed down up\n");break;
          case SDLK_z:  CLEAR(0x01,rows[0]);write_log("  you pressed down A\n");break;
          case SDLK_x:  CLEAR(0x02,rows[0]);write_log("  you pressed down B\n");break;
          case SDLK_a:  CLEAR(0x08,rows[0]);write_log("  you pressed down start\n");break;
          case SDLK_s:  CLEAR(0x04,rows[0]);write_log("  you pressed down select\n");break;
          case SDLK_q:  return 1;
        }
        break;
      }
      case SDL_KEYUP:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_LEFT:  SET(0x02,rows[1]);write_log("  you lifted left\n");break;
          case SDLK_RIGHT:  SET(0x01,rows[1]);write_log("  you lifted right\n");break;
          case SDLK_DOWN:  SET(0x08,rows[1]);write_log("  you lifted down\n");break;
          case SDLK_UP:  SET(0x04,rows[1]);write_log("  you lifted up\n");break;
          case SDLK_z:  SET(0x01,rows[0]);write_log("  you lifted A\n");break;
          case SDLK_x:  SET(0x02,rows[0]);write_log("  you lifted B\n");break;
          case SDLK_a:  SET(0x08,rows[0]);write_log("  you lifted start\n");break;
          case SDLK_s:  SET(0x04,rows[0]);write_log("  you lifted select\n");break;
        }
        break;
      }
    }
  }
}