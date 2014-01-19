#include "gpu.h"
#include "globals.h"

SDL_Surface *init_gpu(void)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface *screen = NULL;
  screen = SDL_SetVideoMode(160,140,32,SDL_SWSURFACE);
  return screen;
}

void update_palette(uint8 palette, uint8 value)
{
  int i,j;
  if (palette == 0)
  {
    j = 0;
    for (i = 0x03; i < 0xFF; i = i << 2)
    {
      switch(GET(i,value) >> (j << 1))
      {
        case 0: {pal_obp0[j] = SDL_MapRGBA(gameboy->screen->format,0xFF,0xFF,0xFF,0xFF);break;}
        case 1: {pal_obp0[j] = SDL_MapRGBA(gameboy->screen->format,0xC0,0xC0,0xC0,0xFF);break;}
        case 2: {pal_obp0[j] = SDL_MapRGBA(gameboy->screen->format,0x60,0x60,0x60,0xFF);break;}
        case 3: {pal_obp0[j] = SDL_MapRGBA(gameboy->screen->format,0x00,0x00,0x00,0xFF);break;}
      }
      j++;
    }
  }
  else if (palette == 1)
  {
    j = 0;
    for (i=0x03;i<0xFF;i=i<<2)
    {
      switch(GET(i,value)>>(j<<1))
      {
        case 0: {pal_obp1[j] = SDL_MapRGBA(gameboy->screen->format,0xFF,0xFF,0xFF,0xFF);break;}
        case 1: {pal_obp1[j] = SDL_MapRGBA(gameboy->screen->format,0xC0,0xC0,0xC0,0xFF);break;}
        case 2: {pal_obp1[j] = SDL_MapRGBA(gameboy->screen->format,0x60,0x60,0x60,0xFF);break;}
        case 3: {pal_obp1[j] = SDL_MapRGBA(gameboy->screen->format,0x00,0x00,0x00,0xFF);break;}
      }
      j++;
    }
  }

  else if (palette == 2)
  {
    j = 0;
    for (i=0x03;i<0xFF;i=i<<2)
    {
      switch(GET(i,value)>>(j<<1))
      {
        case 0: {pal_bgp[j] = SDL_MapRGBA(gameboy->screen->format,0xFF,0xFF,0xFF,0xFF);break;}
        case 1: {pal_bgp[j] = SDL_MapRGBA(gameboy->screen->format,0xC0,0xC0,0xC0,0xFF);break;}
        case 2: {pal_bgp[j] = SDL_MapRGBA(gameboy->screen->format,0x60,0x60,0x60,0xFF);break;}
        case 3: {pal_bgp[j] = SDL_MapRGBA(gameboy->screen->format,0x00,0x00,0x00,0xFF);break;}
      }
      j++;
    }
  }
}
