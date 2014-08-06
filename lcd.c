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
        if (IO(_LY) < 145)
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
        draw_sprites();
        int i;
        Uint32 color;
        Uint32 *pixels = (Uint32 *)gameboy->lcd.screen->pixels;
        for (i = 0; i < 160; i++)
        {
          //color = pal_bgp[gameboy->lcd.linebuffer[i]];
          if (!(IO(_LCDC) & 0x21)) {color = SDL_MapRGB(gameboy->lcd.screen->format,0xFF,0xFF,0xFF);}
          if (IO(_LCDC) & 0x01) {color = pal_bgp[gameboy->lcd.linebuffer[i] & 0x03];}
          if (IO(_LCDC) & 0x20) {color = pal_bgp[gameboy->lcd.linebuffer[i] >> 2];if (color == SDL_MapRGB(gameboy->lcd.screen->format,0xFF,0xFF,0xFF)) {color = pal_bgp[gameboy->lcd.linebuffer[i] & 0x03];}}
          pixels[(IO(_LY) * gameboy->lcd.screen->w) + i] = color;
        }
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
  //get offset within tile map for leftmost tile
  //make mapoffset = line number then shift right 5 because there are 32 bytes in each line
  uint16 mapoffset = (IO(_LY)/8);
  //add scy part that moves lines
  mapoffset += (IO(_SCY)/8);
  mapoffset &= 31;
  mapoffset <<= 5;
  //add scx part that moves tiles
  mapoffset += ((IO(_SCX)/8) & 31);
  //read tile map number
  uint8 t_map_number = ((IO(_LCDC) & 0x08) ? T_MAP_1(mapoffset) : T_MAP_0(mapoffset));
  //fix tile number
  if (!(IO(_LCDC) & 0x10))
  {
    if (t_map_number > 127) {t_map_number -= 128;}
    else if (t_map_number <= 127) {t_map_number += 128;}
  }
  //get offset within tile
  uint8 x = IO(_SCX) & 7;
  uint8 y = (IO(_LY) + IO(_SCY)) & 7;
  //get tile line data using tile map number
  uint16 t_data = ((IO(_LCDC) & 0x10) ? T_DATA_0(16*t_map_number + 2*y) : T_DATA_1(16*t_map_number + 2*y));
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
      mapoffset = (IO(_LY)/8);
      mapoffset += (IO(_SCY)/8);
      mapoffset &= 31;
      mapoffset <<= 5;
      mapoffset += (((IO(_SCX) + i)/8) & 31);
      uint8 t_map_number = ((IO(_LCDC) & 0x08) ? T_MAP_1(mapoffset) : T_MAP_0(mapoffset));
      if (!(IO(_LCDC) & 0x10))
      {
        if (t_map_number > 127) {t_map_number -= 128;}
        else if (t_map_number <= 127) {t_map_number += 128;}
      }
      uint16 t_data = ((IO(_LCDC) & 0x10) ? T_DATA_0(16*t_map_number + 2*y) : T_DATA_1(16*t_map_number + 2*y));
    }
  }
  }
  else if (!(IO(_LCDC) & 0x01)) {int i; for(i = 0; i < 160; i++) {gameboy->lcd.linebuffer[i] = 0;}}
  if (IO(_LCDC) & 0x20)
  {
    uint8 w_offset = (IO(_LY)/8 & 31) << 5;
    uint8 w_map_number = ((IO(_LCDC) & 0x40) ? T_MAP_1(w_offset) : T_MAP_0(w_offset));
    //offsets within tile
    uint8 x = 0;
    uint8 y = IO(_LY) & 7;
    uint16 w_data = ((IO(_LCDC) & 0x10) ? T_DATA_0(16*w_map_number + 2*y) : T_DATA_1(16*w_map_number + 2*y));
    int i;
    for (i = 0; i < 160; i++)
    {
      uint8 a = (LOW(w_data) & BIT(x)) >> x;
      uint8 b = (HIGH(w_data) & BIT(x)) >> x;
      uint8 c =  (a << 2) + (b << 3);
      gameboy->lcd.linebuffer[i] += c;
      x++;
      if (x == 8)
      {
        x = 0;
        w_offset = (IO(_LY)/8 & 31) << 5;
        uint8 w_map_number = ((IO(_LCDC) & 0x40) ? T_MAP_1(w_offset) : T_MAP_0(w_offset));
        uint16 w_data = ((IO(_LCDC) & 0x10) ? T_DATA_0(16*w_map_number + 2*y) : T_DATA_1(16*w_map_number + 2*y));
      }
    }
  }
  else if (!(IO(_LCDC) & 0x20)) {int i; for(i = 0; i < 160; i++) {gameboy->lcd.linebuffer[i] += 0;}}
}

void draw_sprites(void)
{
  int i;
  int x = 0;
  int y = 0;
  int count = 0;
  int size = (IO(_LCDC) & 0x04);

  if (!(IO(_LCDC) & 0x80)) return;
  if (IO(_LCDC) & 0x02)
  {
    int yc = IO(_LY);
    int address = 0;
    for (i = 0; i < 40; i++)
    {
      y = OAM(address);
      address++;
      x = OAM(address);
      address++;
      int tile = OAM(address);
      address++;
      if (size) tile &= 254;
      int flags = OAM(address);
      address++;
      if (x > 0 && y > 0 && x < 168 && y < 160)
      {
        int t = yc - y + 16;
        if (size && t >= 0 && t < 16)
        {
          draw_sprite_tile(tile,x-8,yc,t,flags,size,i);
          count++;
        }
        else if (!size && t >= 0 && t < 8)
        {
          draw_sprite_tile(tile,x-8,yc,t,flags,size,i);
          count++;
        }
      }
      if (count >= 10) break;
    }
  }
}

void draw_sprite_tile(int tile, int x, int y, int t, int flags, int size, int number)
{
  Uint32 *pal = pal_obp0;
  uint8 c;
  if (flags & 0x10) pal = pal_obp1;
  int flipx = (flags & 0x20);
  int flipy = (flags & 0x40);
  if (flipy)
  {
    t = (size ? 15 : 7) - t;
  }
  int priority = (flags & 0x80);
  int address = tile * 16 + 2 * t;
  int a = 0;
  int b = 0;
  a = T_DATA_0(address++);
  b = T_DATA_0(address++);
  int xx;
  //x is tile x coordinate
  //xx is x coordinate within tile
  //xxx is x + xx
  for (xx = 0; xx < 8; xx++)
  {
    uint8 mask = 1 << (7 - xx);
    uint8 c = 0;
    if ((a & mask)) c++;
    if ((b & mask)) c += 2;
    if (c == 0) continue;

    int xxx = xx+x;
    if (flipx)
    {
      xxx = (7 - xx + x);
    }
    if (xxx < 0 || xxx > 159) continue;
    gameboy->lcd.linebuffer[xxx] = c;// pal[c];
  }
}
