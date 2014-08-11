#include "lcd.h"
#include "globals.h"

lcd init_lcd(void)
{
  lcd g;
  SDL_Init(SDL_INIT_EVERYTHING);
  g.screen = NULL;
  g.screen = SDL_SetVideoMode(160,144,32,SDL_HWSURFACE);
  g.clk = 0;
  return g;
}
void compareLYtoLYC(void)
{
  //if equal
  if (IO(_LY) == IO(_LYC))
  {
    //set coincidence bit
    IO(_LCDSTAT) |= 0x04;
    //if interrupt enabled
    if (IO(_LCDSTAT) & 0x40) REQUEST_INT(INT_LCD);
  }
  //if not equal
  else
  {
    //clear coincidence bit
    IO(_LCDSTAT) &= 0xFB;
  }
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
        if (IO(_LY) < 144)
        {
          SET_MODE_OAM;
          gameboy->lcd.clk += T_OAM;
        }
        else //if (IO(_LY) == 145) on last line of screen
        {
          //not needed?
          //interrupt(INT_VBL);
          REQUEST_INT(INT_VBL);
          SET_MODE_VBLANK;
          gameboy->lcd.clk += T_LY_INC;
        }
        break;
      }
      case MODE_VBLANK:
      {
        SDL_Flip(gameboy->lcd.screen);
        compareLYtoLYC();
        if (IO(_LY) < 155)
        {
          IO(_LY)++;
          gameboy->lcd.clk += T_LY_INC;
        }
        else //if (IO(_LY) == 155) on last line of vblank
        {
          IO(_LY) = 1;
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
        if (IO(_LCDC) & 0x80)
        {
          int i;
          Uint32 color;
          Uint32 *pixels = (Uint32 *)gameboy->lcd.screen->pixels;
          for (i = 0; i < 160; i++)
          {
            //color = pal_bgp[gameboy->lcd.linebuffer[i]];
            if (!(IO(_LCDC) & 0x21)) {color = SDL_MapRGB(gameboy->lcd.screen->format,0xFF,0xFF,0xFF);}
            if (IO(_LCDC) & 0x01 && !(IO(_LCDC) & 0x20)) {color = pal_bgp[gameboy->lcd.linebuffer[i] & 0x03];}
            if (IO(_LCDC) & 0x20 && !(IO(_LCDC) & 0x01)) {color = pal_bgp[gameboy->lcd.linebuffer[i] >> 2];}
            if (IO(_LCDC) & 0x21) {if ((gameboy->lcd.linebuffer[i] >> 2) == 0) {color = pal_bgp[gameboy->lcd.linebuffer[i] & 0x03];} else {color = pal_bgp[gameboy->lcd.linebuffer[i] >> 2];}}
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

void draw_line(void)
{
  if (IO(_LCDC) & 0x01) {
  //line number in tiles mod 32 then multiply by 32 because 32 tiles/bytes per line then add scx in tiles mod 32
  uint16 mapoffset = ((((IO(_LY) + IO(_SCY)) >> 3) & 31) << 5) + ((IO(_SCX) >> 3) & 31);
  uint8 t_map_number = ((IO(_LCDC) & 0x08) ? T_MAP_1(mapoffset) : T_MAP_0(mapoffset));
  //fix tile number
  if (!(IO(_LCDC) & 0x10))
  {
    if (t_map_number > 127) {t_map_number -= 128;}
    else {t_map_number += 128;}
  }
  //get offset within tile
  uint8 x = IO(_SCX) & 7;
  uint8 y = (IO(_LY) + IO(_SCY)) & 7;
  //get tile line data using tile map number
  uint16 t_data = ((IO(_LCDC) & 0x10) ? T_DATA_0(16*t_map_number + (y << 1)) : T_DATA_1(16*t_map_number + (y << 1)));
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
      uint8 t_map_number = ((IO(_LCDC) & 0x08) ? T_MAP_1(mapoffset) : T_MAP_0(mapoffset));
      if (!(IO(_LCDC) & 0x10))
      {
        if (t_map_number > 127) {t_map_number -= 128;}
        else {t_map_number += 128;}
      }
      uint16 t_data = ((IO(_LCDC) & 0x10) ? T_DATA_0(16*t_map_number + (y << 1)) : T_DATA_1(16*t_map_number + (y << 1)));
    }
  }
  }
  else if (!(IO(_LCDC) & 0x01)) {int i; for(i = 0; i < 160; i++) {gameboy->lcd.linebuffer[i] = 0;}}
  if (IO(_LCDC) & 0x20 && IO(_WY) <= IO(_LY))
  {
    uint8 w_offset = (((IO(_LY) - IO(_WY)) >> 3) & 31) << 5;
    uint8 w_map_number = ((IO(_LCDC) & 0x40) ? T_MAP_1(w_offset) : T_MAP_0(w_offset));
    //offsets within tile
    int x = 0;
    uint8 y = (IO(_LY) - IO(_WY)) & 7;
    uint16 w_data = ((IO(_LCDC) & 0x10) ? T_DATA_0(16*w_map_number + (y << 1)) : T_DATA_1(16*w_map_number + (y << 1)));
    int i;
    for (i = IO(_WX)-7; i < 160; i++)
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
        uint8 w_map_number = ((IO(_LCDC) & 0x40) ? T_MAP_1(w_offset) : T_MAP_0(w_offset));
        uint16 w_data = ((IO(_LCDC) & 0x10) ? T_DATA_0(16*w_map_number + (y << 1)) : T_DATA_1(16*w_map_number + (y << 1)));
      }
    }
  }
  //next line is redundant
  //else if (!(IO(_LCDC) & 0x20)) {int i; for(i = 0; i < 160; i++) {gameboy->lcd.linebuffer[i] += 0;}}
}

//reverses bits in each byte of 16 bit word doesn't change byte order, used for drawing sprites
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

void draw_sprites(void)
{
  if (IO(_LCDC) & 0x02)
  {
    int i = 0;
    int count = 0;
    for (i = 0; i < 40; i++)
    {
      if (OAM(i,OAM_Y) - 16 <= IO(_LY) && (OAM(i,OAM_Y) - 16 + ((IO(_LCDC) & 0x04) ? 16 : 8)) > IO(_LY))
      {
        uint8 y = ((IO(_LY) - OAM(i,OAM_Y)) & 7) << 1;
        uint8 yflip = (7 - ((IO(_LY) - OAM(i,OAM_Y)) & 7)) << 1;
        uint8 t_number = OAM(i,OAM_TILE);
        uint16 t_data;
        if (!(IO(_LCDC) & 0x04))//8x8 mode
        {
          t_data = T_DATA_0(16*t_number + ((OAM(i,OAM_FLAGS) & 0x40) ? yflip : y) );
        }
        else//8x16 mode
        {
          t_data = T_DATA_0(16 * (((IO(_LY) - OAM(i,OAM_Y)) > 7) ? t_number | 0x01 : t_number & 0xFE) + ((OAM(i,OAM_FLAGS) & 0x40) ? yflip : y) );
        }
        int x = 0;
        count++;
        for (x = 0; x < 8; x++)
        {
          if (OAM(i,OAM_X) + x - 8 >= 0 && OAM(i,OAM_X) + x - 8  < 160 && (!(OAM(i,OAM_FLAGS) & 0x80) || (gameboy->lcd.linebuffer[OAM(i,OAM_X) + x - 8] & 0x03) == 0))
          {
            if (OAM(i,OAM_FLAGS) & 0x20) t_data = reverse_word(t_data);
            uint8 a = (LOW(t_data) & BIT(x)) >> x;
            uint8 b = (HIGH(t_data) & BIT(x)) >> x;
            uint8 c =  a + (b << 1);
            if (c != 0)
            {
              Uint32 color;
              Uint32 *pixels = (Uint32 *)gameboy->lcd.screen->pixels;
              color = (OAM(i,OAM_FLAGS) & 0x10 ? pal_obp1[c] : pal_obp0[c]);
              pixels[IO(_LY) * gameboy->lcd.screen->w + OAM(i,OAM_X) + x - 8] = color;
            }
          }
        }
      }
      if (count >= 10) break;
    }
  }
}
