#ifndef GPU_H
#define GPU_H
#include "globals.h"

extern SDL_Surface *init_gpu(void);
extern void step_gpu(void);
extern void update_palette(uint8,uint8);

//useful abstractions for handling palettes
Uint32 pal_obp0[4];
Uint32 pal_obp1[4];
Uint32 pal_bgp[4];

#endif