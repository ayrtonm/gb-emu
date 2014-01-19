#ifndef GPU_H
#define GPU_H
#include "globals.h"

extern gpu init_gpu(void);
extern void step_gpu(uint8);
extern void update_palette(uint8,uint8);
//draw_screen() should only be called from step_gpu()
void draw_screen(void);

//useful abstractions for handling palettes
Uint32 pal_obp0[4];
Uint32 pal_obp1[4];
Uint32 pal_bgp[4];

#endif