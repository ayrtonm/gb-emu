#include "lcd.h"
#include "globals.h"
#undef GRID

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
///*
          if ((!(IO(_LCDSTAT) & 0x40)) || (IO(_LY) != IO(_LYC)))
          {
            if (IO(_LCDSTAT) & 0x28 == 0x20) REQUEST_INT(INT_LCD);
          }
//*/
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
///*
          if ((!(IO(_LCDSTAT) & 0x40)) || (IO(_LY) != IO(_LYC)))
          {
            if (IO(_LCDSTAT) & 0x28 == 0x20) REQUEST_INT(INT_LCD);
          }
//*/
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
          color = pal_bgp[gameboy->lcd.linebuffer[i]];
          pixels[(IO(_LY) * gameboy->lcd.screen->w) + i] = color;
        }
//        SDL_Flip(gameboy->lcd.screen);
///*
        if ((!(IO(_LCDSTAT) & 0x40)) || (IO(_LY) != IO(_LYC)))
        {
          if (IO(_LCDSTAT) & 0x28 == 0x20) REQUEST_INT(INT_LCD);
        }
//*/
        SET_MODE_HBLANK;
        gameboy->lcd.clk = T_HBLANK;
        break;
      }
    }
  }
}

void draw_line(void)
{
  //'vertical' offset in tile map
  uint16 mapoffs = IO(_LY) + IO(_SCY);
  mapoffs &= 0xFF;
  mapoffs >>= 3;
  mapoffs <<= 5;

  //'horizontal' offset in tile map
  uint16 lineoffs = (IO(_SCX) >> 3) & 31;

  //what tile to start off with
  uint16 index = (IO(_LCDC) & 0x08 ? T_MAP_1(mapoffs+lineoffs) : T_MAP_0(mapoffs+lineoffs));

  //where in the tile to start
  uint8 x = IO(_SCX) & 7;
  uint8 y = (IO(_LY) + IO(_SCY)) & 7;

  //fix tile data for certain tile set
  if (!(IO(_LCDC) & 0x10))
  {
    if (index < 128) index += 128;
    else index -= 128;
  }

  int i;
  for (i = 0; i < 160; i++)
  {
    //get 16-bit tile data
    uint16 tile = (IO(_LCDC) & 0x10 ? T_DATA_0(index) : T_DATA_1(index));

    //mask all but 0xnn where n = 'pixel number'
    //then add the 2 bits
    uint8 a = (LOW(tile) & BIT(x)) >> x;
    uint8 b = (HIGH(tile) & BIT(x)) >> x;
    uint8 c = a+(b>>1);

    //write to linebuffer
    gameboy->lcd.linebuffer[i] = c;//pal_bgp[pixel];

    //increment x pixel within tile
    x++;

    if (x == 8)
    {
      //reset x pixel within tile, get new tile data
      x = 0;
      lineoffs = (lineoffs + 1) & 31;
      index = (IO(_LCDC) & 0x08 ? T_MAP_1(mapoffs+lineoffs) : T_MAP_0(mapoffs+lineoffs));
      //if (!(IO(_LCDC) & 0x10)) index += 0xFF;
    }
  }
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
