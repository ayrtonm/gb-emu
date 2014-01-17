#ifndef GPU_H
#define GPU_H
#include <SDL/SDL.h>
#include "mem.h"
#include "z80.h" //for bit manipulation

SDL_Surface* screen;
uint16 m_clk;

Uint32 pal_obp0[4];
Uint32 pal_obp1[4];
Uint32 pal_bgp[4];

extern SDL_Surface * init_gpu(void);
extern void step_gpu(void);
extern void update_palette(uint8,uint8);

#endif