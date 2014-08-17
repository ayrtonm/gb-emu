#include "lcd.h"
//the following macro definition is only for debugging drawing sprites and will eventually be removed
//#define GRID
//the following macro is a placeholder for a configuration file or run-time arguments
#define DEFAULT_SCALE 3

/**
  makes lcd struct and initializes SDL Surface used for screen and lcd clock
**/
lcd init_lcd(void)
{
  lcd g;
  SDL_Init(SDL_INIT_EVERYTHING);
//  g.screen = NULL;
  g.screen = SDL_CreateRGBSurface(SDL_HWSURFACE,160,144,32,0,0,0,0);
  g.visible = SDL_SetVideoMode(160*DEFAULT_SCALE,144*DEFAULT_SCALE,32,SDL_HWSURFACE|SDL_RESIZABLE);
  SDL_WM_SetCaption("Game Boy Emulator",NULL);
  g.clk = 0;
  return g;
}

/**
  compares current line(LY) to io register at 0xFF45(LYC)
  if they're the same it sets the coincidence bit in lcdstat and request an interrupt if it's enabled by lcdstat
**/
void compareLYtoLYC(void)
{
  if (IO(_LY) == IO(_LYC))
  {
    IO(_LCDSTAT) |= 0x04;
    if (IO(_LCDSTAT) & 0x40) REQUEST_INT(INT_LCD);
  }
  else
  {
    IO(_LCDSTAT) &= 0xFB;
  }
}

/**
  updates palettes for background(bgp) and for sprites obp0/1
  palettes are only updated when writing to the address of the palettes
**/
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
        case 0: {pal_obp0[j] = SDL_MapRGB(gameboy->lcd.screen->format,0xFF,0xFF,0xFF);break;}
        case 1: {pal_obp0[j] = SDL_MapRGB(gameboy->lcd.screen->format,0xC0,0xC0,0xC0);break;}
        case 2: {pal_obp0[j] = SDL_MapRGB(gameboy->lcd.screen->format,0x60,0x60,0x60);break;}
        case 3: {pal_obp0[j] = SDL_MapRGB(gameboy->lcd.screen->format,0x00,0x00,0x00);break;}
      }
      j++;
    }
  }
  else if (palette == 1)
  {
    j = 0;
    for (i = 0x03; i < 0xFF; i = i << 2)
    {
      switch(GET(i,value) >> (j << 1))
      {
        case 0: {pal_obp1[j] = SDL_MapRGB(gameboy->lcd.screen->format,0xFF,0xFF,0xFF);break;}
        case 1: {pal_obp1[j] = SDL_MapRGB(gameboy->lcd.screen->format,0xC0,0xC0,0xC0);break;}
        case 2: {pal_obp1[j] = SDL_MapRGB(gameboy->lcd.screen->format,0x60,0x60,0x60);break;}
        case 3: {pal_obp1[j] = SDL_MapRGB(gameboy->lcd.screen->format,0x00,0x00,0x00);break;}
      }
      j++;
    }
  }

  else if (palette == 2)
  {
    j = 0;
    for (i = 0x03; i < 0xFF; i = i << 2)
    {
      switch(GET(i,value) >> (j << 1))
      {
        case 0: {pal_bgp[j] = SDL_MapRGB(gameboy->lcd.screen->format,0xFF,0xFF,0xFF);break;}
        case 1: {pal_bgp[j] = SDL_MapRGB(gameboy->lcd.screen->format,0xC0,0xC0,0xC0);break;}
        case 2: {pal_bgp[j] = SDL_MapRGB(gameboy->lcd.screen->format,0x60,0x60,0x60);break;}
        case 3: {pal_bgp[j] = SDL_MapRGB(gameboy->lcd.screen->format,0x00,0x00,0x00);break;}
      }
      j++;
    }
  }
}

/**
  steps through the different lcd modes based on elapsed time between opcodes(4 * dt)
  basically adds T_mode to clk depending on what mode just ended and decreases clk based on dt if clk <= 0 mode changes
  the grid stuff will eventually be removed
  screen is flipped during vblank
  vram and oam mode don't emulate the hw exactly since everything on each line is drawn right at the end of vram mode since that's the simplest way to do it, maybe that's what's causing the glitches in the spaceship game
**/
void step_lcd(uint8 dt)
{
  gameboy->lcd.clk -= 4*dt;
  if (gameboy->lcd.clk <= 0)
  {
    switch(LCD_MODE)
    {
      case MODE_HBLANK:
      {
        IO(_LY)++;
        compareLYtoLYC();
        if (IO(_LY) < 143)
        {
          SET_MODE_OAM;
          gameboy->lcd.clk += T_OAM;
        }
        else
        {
          REQUEST_INT(INT_VBL);
          SET_MODE_VBLANK;
          gameboy->lcd.clk += T_LY_INC;
        }
        break;
      }
      case MODE_VBLANK:
      {
#ifdef GRID
        //draw a grid
        int i,j;
        for (i = 0; i < 20; i++)
        {
          for (j = 0; j < 18; j++)
          {
            SDL_Rect dst = {(i*8),(j*8),160,1};
            SDL_FillRect(gameboy->lcd.screen,&dst,SDL_MapRGB(gameboy->lcd.screen->format,0x00,0xFF,0x00));
          }
          SDL_Rect dst = {(i*8),0,1,144};
          SDL_FillRect(gameboy->lcd.screen,&dst,SDL_MapRGB(gameboy->lcd.screen->format,0x00,0xFF,0x00));
        }
#endif
        SDL_BlitSurface(zoomSurface(gameboy->lcd.screen,(float)gameboy->lcd.visible->w/160.0,(float)gameboy->lcd.visible->h/144.0,0),NULL,gameboy->lcd.visible,NULL);
        SDL_Flip(gameboy->lcd.visible);
        compareLYtoLYC();
        if (IO(_LY) < 153)
        {
          IO(_LY)++;
          gameboy->lcd.clk += T_LY_INC;
        }
        else
        {
          IO(_LY) = 0;
          SET_MODE_OAM;
          gameboy->lcd.clk += T_OAM;
        }
        break;
      }
      case MODE_OAM:
      {
        SET_MODE_VRAM;
        gameboy->lcd.clk += T_VRAM;
        break;
      }
      case MODE_VRAM:
      {
        compareLYtoLYC();
        draw_line();
        if (IO(_LCDC) & LCDC_ENABLE)
        {
          int i;
          Uint32 color;
          Uint32 *pixels = (Uint32 *)gameboy->lcd.screen->pixels;
          for (i = 0; i < 160; i++)
          {
            if (!(IO(_LCDC) & (LCDC_WIN_ENABLE|LCDC_BG_ENABLE)))
            {
              color = SDL_MapRGB(gameboy->lcd.screen->format,0xFF,0xFF,0xFF);
            }
            else if (IO(_LCDC) & LCDC_BG_ENABLE && !(IO(_LCDC) & LCDC_WIN_ENABLE))
            {
              color = pal_bgp[gameboy->lcd.linebuffer[i] & 0x03];
            }
            else if (IO(_LCDC) & LCDC_WIN_ENABLE && !(IO(_LCDC) & LCDC_BG_ENABLE))
            {
              color = pal_bgp[gameboy->lcd.linebuffer[i] >> 2];
            }
            else if (IO(_LCDC) & (LCDC_WIN_ENABLE|LCDC_BG_ENABLE))
            {
              if ((gameboy->lcd.linebuffer[i] >> 2) == 0)
              {
                color = pal_bgp[gameboy->lcd.linebuffer[i] & 0x03];
              }
              else
              {
                color = pal_bgp[gameboy->lcd.linebuffer[i] >> 2];
              }
            }
            pixels[(IO(_LY) * gameboy->lcd.screen->w) + i] = color;
          }
        }
        draw_sprites();
        SET_MODE_HBLANK;
        gameboy->lcd.clk = T_HBLANK;
        break;
      }
    }
  }
}

/**
  draws background and window for each line
  since a lot of multiplication here is by 2^n a lot of bit shifting is used
  mapoffset is current y position in terms of tiles mod 32 because scrolling bkg then multiplied by 32 because 32 tiles(bytes) per line then add scx in tiles mod 32
  t_map_number is tile number depending on which tile map is being used, then tile number is fixed
  uint8 x and y are for offset within tile
  tile data is read using tile map number and y
  tile data is read two bits at a time
  ex:  axxx xxxx
       bxxx xxxx
  top line is high byte of tile data, bottom line is low byte
  the color number for the leftmost pixel would be 2*a + b
  color number is written to the linebuffer and written at the end of vram
  repeat for each pixel in tile, then tile mapoffset is updated and tile data is read again for next tile
  basically the same thing for drawing the window except scy and scx are wy and wx and they're subtracted not added to the offset, so the window is always drawn starting at its top left corner but it may be drawn anywhere on the screen
**/
void draw_line(void)
{
  if (IO(_LCDC) & LCDC_BG_ENABLE)
  {
    uint16 mapoffset = ((((IO(_LY) + IO(_SCY)) >> 3) & 31) << 5) + ((IO(_SCX) >> 3) & 31);
    uint8 t_map_number = ((IO(_LCDC) & LCDC_BG_MAP) ? T_MAP_1(mapoffset) : T_MAP_0(mapoffset));
    if (!(IO(_LCDC) & LCDC_BG_DATA))
    {
      if (t_map_number > 127) {t_map_number -= 128;}
      else {t_map_number += 128;}
    }
    uint8 x = IO(_SCX) & 7;
    uint8 y = (IO(_LY) + IO(_SCY)) & 7;
    uint16 t_data = ((IO(_LCDC) & LCDC_BG_DATA) ? T_DATA_0(16*t_map_number + (y << 1)) : T_DATA_1(16*t_map_number + (y << 1)));
    int i;
    for (i = 0; i < 160; i++)
    {
      uint8 a = (LOW(t_data) & BIT(x)) >> x;
      uint8 b = (HIGH(t_data) & BIT(x)) >> x;
      uint8 c = a + (b << 1);
      gameboy->lcd.linebuffer[i] = c;
      x++;
      if (x == 8)
      {
        x = 0;
        mapoffset = ((((IO(_LY) + IO(_SCY)) >> 3) & 31) << 5) + (((IO(_SCX) + i) >> 3) & 31);
        uint8 t_map_number = ((IO(_LCDC) & LCDC_BG_MAP) ? T_MAP_1(mapoffset) : T_MAP_0(mapoffset));
        if (!(IO(_LCDC) & LCDC_BG_DATA))
        {
          if (t_map_number > 127) {t_map_number -= 128;}
          else {t_map_number += 128;}
        }
        uint16 t_data = ((IO(_LCDC) & LCDC_BG_DATA) ? T_DATA_0(16*t_map_number + (y << 1)) : T_DATA_1(16*t_map_number + (y << 1)));
      }
    }
  }
  else if (!(IO(_LCDC) & LCDC_BG_ENABLE)) {int i; for(i = 0; i < 160; i++) {gameboy->lcd.linebuffer[i] = 0;}}
  if (IO(_LCDC) & LCDC_WIN_ENABLE && IO(_WY) <= IO(_LY))
  {
    uint8 w_offset = (((IO(_LY) - IO(_WY)) >> 3) & 31) << 5;
    uint8 w_map_number = ((IO(_LCDC) & LCDC_WIN_MAP) ? T_MAP_1(w_offset) : T_MAP_0(w_offset));
    //offsets within tile
    int x = 0;
    uint8 y = (IO(_LY) - IO(_WY)) & 7;
    uint16 w_data = T_DATA_1(16*w_map_number + (y << 1));
    int i;
    for (i = MAX(IO(_WX)-7,0); i < 160; i++)
    {
      uint8 a = (LOW(w_data) & BIT(x)) >> x;
      uint8 b = (HIGH(w_data) & BIT(x)) >> x;
      uint8 c =  (a << 2) + (b << 3);
      gameboy->lcd.linebuffer[i] += c;
      x++;
      if (x == 8)
      {
        x = 0;
        w_offset = (((IO(_LY)-IO(_WY)) >> 3) & 31) << 5;
        uint8 w_map_number = ((IO(_LCDC) & LCDC_WIN_MAP) ? T_MAP_1(w_offset) : T_MAP_0(w_offset));
        uint16 w_data = T_DATA_1(16*w_map_number + (y << 1));
      }
    }
  }
}

/**
  reverses each byte in a 16-bit word, used for drawing horizontally flipped sprites
**/
uint16 reverse_word(uint16 input)
{
  uint8 a = LOW(input);
  uint8 b = HIGH(input);
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  a = (a & 0xF0) >> 4 | (a & 0x0F) << 4;
  a = (a & 0xCC) >> 2 | (a & 0x33) << 2;
  a = (a & 0xAA) >> 1 | (a & 0x55) << 1;
  return a + (b << 8);
}

/**
  draws sprites like the name implies
**/
void draw_sprites(void)
{
  if (IO(_LCDC) & LCDC_OBJ_ENABLE)
  {
    int i = 0;
    int count = 0;
    for (i = 0; i < OAM_MAX; i++)
    {
      if (OAM(i,OAM_Y) - 16 <= IO(_LY) && (OAM(i,OAM_Y) - 16 + ((IO(_LCDC) & LCDC_OBJ_SIZE) ? 16 : 8)) > IO(_LY))
      {
        uint8 y = ((IO(_LY) - OAM(i,OAM_Y) + 16) & 7) << 1;
        uint8 yflip = (7 - ((IO(_LY) - OAM(i,OAM_Y) + 16) & 7)) << 1;
        uint8 t_number = OAM(i,OAM_TILE);
        uint16 t_data;
        if (!(IO(_LCDC) & LCDC_OBJ_SIZE))//8x8 mode
        {
          t_data = T_DATA_0(16*t_number + ((OAM(i,OAM_FLAGS) & OAM_F_YFLIP) ? yflip : y) );
        }
        else//8x16 mode
        {
          t_data = T_DATA_0(16 * (((IO(_LY) - OAM(i,OAM_Y) + 16) > 7) || !(yflip) ? OAM_LO_TILE(t_number) : OAM_HI_TILE(t_number)) + ((OAM(i,OAM_FLAGS) & OAM_F_YFLIP) ? yflip : y) );
        }
        if (!(OAM(i,OAM_FLAGS) & OAM_F_XFLIP)) {t_data = reverse_word(t_data);}
        int x = 0;
        count++;
        for (x = 0; x < 8; x++)
        {
          if (OAM(i,OAM_X) + x - 8 >= 0 && OAM(i,OAM_X) + x - 8  < 160 && (!(OAM(i,OAM_FLAGS) & OAM_F_BG) || (gameboy->lcd.linebuffer[OAM(i,OAM_X) + x - 8] & 0x03) == 0))
          {
            uint8 a = (LOW(t_data) & BIT(x)) >> x;
            uint8 b = (HIGH(t_data) & BIT(x)) >> x;
            uint8 c =  a + (b << 1);
            if (c != 0)
            {
              Uint32 color;
              Uint32 *pixels = (Uint32 *)gameboy->lcd.screen->pixels;
              color = (OAM(i,OAM_FLAGS) & OAM_F_PAL ? pal_obp1[c] : pal_obp0[c]);
              pixels[IO(_LY) * gameboy->lcd.screen->w + OAM(i,OAM_X) + x - 8] = color;
            }
          }
        }
      }
      if (count >= OAM_MAX_LINE) break;
    }
  }
}
