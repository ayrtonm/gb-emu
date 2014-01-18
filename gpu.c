#include "gpu.h"
#include "globals.h"

SDL_Surface *init_gpu(void)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface *screen = NULL;
  screen = SDL_SetVideoMode(160,140,32,SDL_SWSURFACE);
  return screen;
}