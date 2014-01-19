#include "lcd.h"
#include "globals.h"

lcd init_lcd(void)
{
  lcd g;
  SDL_Init(SDL_INIT_EVERYTHING);
  g.screen = NULL;
  g.screen = SDL_SetVideoMode(160,140,32,SDL_SWSURFACE);
  return g;
}
void compareLYtoLYC(void)
{
  if (IO(_LY) == IO(_LYC))
  {
    IO(_LCDSTAT) |= 4;
    if (IO(_LCDSTAT) & 0x40) IO(_IF) |= INT_LCD;
  }
  else
  {
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
        case 0: {pal_obp0[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0xFF,0xFF,0xFF,0xFF);break;}
        case 1: {pal_obp0[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0xC0,0xC0,0xC0,0xFF);break;}
        case 2: {pal_obp0[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0x60,0x60,0x60,0xFF);break;}
        case 3: {pal_obp0[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0x00,0x00,0x00,0xFF);break;}
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
        case 0: {pal_obp1[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0xFF,0xFF,0xFF,0xFF);break;}
        case 1: {pal_obp1[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0xC0,0xC0,0xC0,0xFF);break;}
        case 2: {pal_obp1[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0x60,0x60,0x60,0xFF);break;}
        case 3: {pal_obp1[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0x00,0x00,0x00,0xFF);break;}
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
        case 0: {pal_bgp[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0xFF,0xFF,0xFF,0xFF);break;}
        case 1: {pal_bgp[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0xC0,0xC0,0xC0,0xFF);break;}
        case 2: {pal_bgp[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0x60,0x60,0x60,0xFF);break;}
        case 3: {pal_bgp[j] = SDL_MapRGBA(gameboy->lcd.screen->format,0x00,0x00,0x00,0xFF);break;}
      }
      j++;
    }
  }
}

void step_lcd(uint8 dt)
{
  gameboy->lcd.clk -= dt;
  if (LCD_MODE == MODE_VBLANK)
  {
    gameboy->lcd.ly -= dt;
    while (gameboy->lcd.ly <= 0)
    {
      gameboy->lcd.ly += T_LY_INC;
      if (IO(_LY) < 153)
      {
        IO(_LY)++;
        compareLYtoLYC();
        if (IO(_LY) >= 153) gameboy->lcd.ly =6;
      }
      else
      {
        IO(_LY) = 0x00;
        //reset the window line
        gameboy->lcd.ly = T_LY_INC * 2;
        compareLYtoLYC();
      }
    }
  }
  while (gameboy->lcd.clk <= 0)
  {
    switch(LCD_MODE)
    {
      case MODE_HBLANK:
      {
        IO(_LY)++;
        compareLYtoLYC();

        //check if reached the Vblank period
        if (IO(_LY) == T_LY_INC)
        {
          //in Vblank
          gameboy->lcd.ly = gameboy->lcd.clk + T_LY_INC;
          gameboy->lcd.clk += T_LCDMODE_1;
          SET_MODE_VBLANK;
          if (IO(_LCDC) & 0x80)
          {
            gameboy->lcd.snooze = 6;
          }
          else
          {
            gameboy->lcd.clk += T_LCDMODE_2;
            SET_MODE_HBLANK;
            if (!(IO(_LCDSTAT) & 0x40) || (IO(_LY) != IO(_LYC)))
            {
              if ((IO(_LCDSTAT) & 0x28) == 0x20) IO(_IF) |= 2;
            }
          }
        }
      }
      break;
      case MODE_VBLANK:
      {
        gameboy->lcd.clk += T_LCDMODE_2;
        SET_MODE_OAM;
        if (!(IO(_LCDSTAT) & 0x40) ||  (IO(_LY) != IO(_LYC)))
        {
          if ((IO(_LCDSTAT) & 0x28) == 0x20) IO(_IF) |= 2;
        }
      }
      break;
      case MODE_OAM:
      {
        gameboy->lcd.clk += T_LCDMODE_3;
        SET_MODE_VRAM;
      }
      break;
      case MODE_VRAM:
      {
        if (IO(_LY) < 144)
        {
          draw_line();
          draw_sprites();
        }
        SET_MODE_HBLANK;
        if (!(IO(_LCDSTAT) & 0x40) ||  (IO(_LY) != IO(_LYC)))
        {
          if ((IO(_LCDSTAT) & 0x28) == 0x20) IO(_IF) |= 2;
        }
        else
        {
          gameboy->lcd.clk += T_LCDMODE_0;
        }
      }
      break;
    }
  }
}

void draw_line(void)
{
  //'vertical' offset in tile map
  uint16 mapoffs = ((IO(_LY) + IO(_SCY)) & 0xFF) >> 3;

  //'horizontal' offset in tile map
  uint16 lineoffs = (IO(_SCX) >> 3);

  //what tile to start off with
  uint16 index = (IO(_LCDC) & 0x08 ? T_MAP_1(mapoffs+lineoffs) : T_MAP_0(mapoffs+lineoffs));

  //where in the tile to start
  uint8 x = IO(_SCX) & 7;
  uint8 y = (IO(_LY) + IO(_SCY)) & 7;

  //fix tile data for certain tile set
  if (!(IO(_LCDC) & 0x10)) index += 0xFF;

  int i;
  for (i = 0; i < 160; i++)
  {
    //get 16-bit tile data
    uint16 tile = (IO(_LCDC) & 0x10 ? T_DATA_0(index) : T_DATA_1(index));

    //mask all but 0x0n0n(or 0xn0n0) where n = 'pixel number'
    //then add the 2 bits
    uint8 pixel = ((LOW(tile) & BIT(x)) << 1) + (HIGH(tile) & BIT(x));

    //load palette
    Uint32 color = pal_bgp[pixel];
    //draw pixel
    Uint32 *pixels = (Uint32 *)gameboy->lcd.screen->pixels;
    pixels[(IO(_LY)*gameboy->lcd.screen->w)+i] = color;

    //increment x pixel within tile
    x++;

    if (x == 8)
    {
      //reset x pixel within tile, get new tile data
      x = 0;
      lineoffs = (lineoffs + 1) & 31;
      index = (IO(_LCDC) & 0x08 ? T_MAP_1(mapoffs+lineoffs) : T_MAP_0(mapoffs+lineoffs));
      if (!(IO(_LCDC) & 0x10)) index += 0xFF;
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
      y = OAM(address++);
      x = OAM(address++);
      int tile = OAM(address++);
      if (size) tile &= 254;
      int flags = OAM(address++);
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
  uint8 *pal = pal_obp0;
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
  int xx;
  for (xx = 0; xx < 8; xx++)
  {
    uint8 mask = 1 << (7 - xx);
    int xxx = xx+x;
    if (flipx)
    {
      xxx = (7 - xx + x);
    }
    if (xxx < 0 || xxx > 159) continue;
    uint16 color = gameboy->lcd.linebuffer[xxx];
    if (priority)
    {
      if (color < 0x200 && (color & 0xFF) != 0) continue;
    }
    if (color >= 0x300 && color != 0x300) continue;
    else if (color >= 0x200 && color < 0x300)
    {
      int sprite = color & 0xFF;
      int spritex = OAM(4 * sprite + 1) - 8;
      if (spritex == x)
      {
        if (sprite < number) continue;
        else if (spritex < x+8) continue;
      }
    }
    gameboy->lcd.linebuffer[xxx] = 0x200 + number;
    c = pal[c];
    c += 4;
    gameboy->lcd.linemix[xxx] = c;
  }
}