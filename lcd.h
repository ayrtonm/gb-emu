#ifndef LCD_H
#define LCD_H
#include "globals.h"

extern lcd init_lcd(void);
extern void step_lcd(uint8);
extern void update_palette(uint8,uint8);
void draw_bkg(void);
void draw_sprites(void);
void draw_sprite_tile(int,int,int,int,int,int,int);

//useful abstractions for handling palettes
Uint32 pal_obp0[4];
Uint32 pal_obp1[4];
Uint32 pal_bgp[4];

#endif