#include "mem.h"
#include "gpu.h"

SDL_Surface * init_gpu(void);
void draw_screen(void);
void step_gpu(void);
void update_palette(uint8 palette, uint8 value);

//may be useful later/elsewhere
//double log2(double n) {return log(n)/log(2);}

//I spent way too much time trying to figure out what was "wrong" with this function
//it was that the switch case statements didn't have break; at the end
//please don't forget the obvious again
void update_palette(uint8 palette, uint8 value)
{
  int i,j;
  if (palette == 0)
  {
    j = 0;
    for (i=0x03;i<0xFF;i=i<<2)
    {
      switch(GET(i,value)>>(j<<1))
      {
        case 0: {pal_obp0[j] = SDL_MapRGBA(screen->format,0xFF,0xFF,0xFF,0xFF);break;}
        case 1: {pal_obp0[j] = SDL_MapRGBA(screen->format,0xC0,0xC0,0xC0,0xFF);break;}
        case 2: {pal_obp0[j] = SDL_MapRGBA(screen->format,0x60,0x60,0x60,0xFF);break;}
        case 3: {pal_obp0[j] = SDL_MapRGBA(screen->format,0x00,0x00,0x00,0xFF);break;}
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
        case 0: {pal_obp1[j] = SDL_MapRGBA(screen->format,0xFF,0xFF,0xFF,0xFF);break;}
        case 1: {pal_obp1[j] = SDL_MapRGBA(screen->format,0xC0,0xC0,0xC0,0xFF);break;}
        case 2: {pal_obp1[j] = SDL_MapRGBA(screen->format,0x60,0x60,0x60,0xFF);break;}
        case 3: {pal_obp1[j] = SDL_MapRGBA(screen->format,0x00,0x00,0x00,0xFF);break;}
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
        case 0: {pal_bgp[j] = SDL_MapRGBA(screen->format,0xFF,0xFF,0xFF,0xFF);break;}
        case 1: {pal_bgp[j] = SDL_MapRGBA(screen->format,0xC0,0xC0,0xC0,0xFF);break;}
        case 2: {pal_bgp[j] = SDL_MapRGBA(screen->format,0x60,0x60,0x60,0xFF);break;}
        case 3: {pal_bgp[j] = SDL_MapRGBA(screen->format,0x00,0x00,0x00,0xFF);break;}
      }
      j++;
    }
  }
}
SDL_Surface * init_gpu(void)
{
  //initialize sdl and set up screen
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface* screen = NULL;
  screen = SDL_SetVideoMode(160,140,32,SDL_SWSURFACE);
  return screen;
}

void draw_screen(void)
{
  //choose which tile map
  uint16 mapoffs = (GET(0x08,lcdc) == 0 ? 0x1800 : 0x1C00);

  //scy plus row in terms of bytes
  mapoffs += ((line + scy) & 0xFF) >> 3;

  //scx in terms of bytes
  uint16 lineoffs = (scx >> 3);

  //what pixel in the tile to start with
  uint8 x = scx & 7;
  uint8 y = (line + scy) & 7;

  //index doesn't include x and y
  write_log("  reading uint16 for index\n");
  uint16 index = read_word((mapoffs+lineoffs));

  //fix indices if tile set is #1
  if (GET(0x10,lcdc)==0) {index += 0xFF;}

  //counter for each pixel in row
  int i;
  for (i = 0; i < 160; i++)
  {
    //tile-set offset
    uint16 setoffs = ((GET(0x10,lcdc) == 0) ? 0x0800 : 0);

    //finally actual tile data
    write_log("  reading uint16 for tile data\n");
    uint16 tile = read_word(index + setoffs);

    //mask all but 0xnn where n = "pixel number" then add the 2 bits
    uint8 pixel = (((tile & 0xFF) & (1 << x)) << 8) + ((tile >> 8) & (1 << x));

    //extremely useful palette abstraction
    Uint32 color = pal_bgp[pixel];

    //draw to buffer
    Uint32 * pixels = (Uint32 *)screen->pixels;
    pixels[(line*screen->w)+i] = color;

    //increment x pixel within tile
    x++;

    //reset x pixel  within tile; get new tile
    if (x == 8)
    {
      x = 0;
      lineoffs = (lineoffs + 1) & 31;
      write_log("  reading uint16 for index\n");
      index = read_word((mapoffs+lineoffs));
      if (GET(0x10,lcdc)==0) {index += 0xFF;}
    }
  }
  int j,jmax,k;
  for (i = 0; i < 40; i++)
  {
    jmax = (GET(0x02,lcdc) == 0 ? 8 : 16);
    for (j = 0; j < jmax; j++)
    {
      for (k = 0; k < 8; k++)
      {
        if (mem[_OAM + (i * 4)]+j >= 16 && mem[_OAM + (i * 4)]+j < 160 && mem[_OAM + (i * 4)+1]+k >= 8 && mem[_OAM + (i * 4)+1]+k < 168)
        {
          uint16 tile = read_word(_VRAM + mem[_OAM + (i*4)+2] + j*2);
          uint8 pixel = (((tile & 0xFF) & (1 << k)) << 8) + ((tile >> 8) & (1 << k));
          Uint32 color;
          if (GET(0x10,mem[_OAM + (i * 4)+3]) == 0) {color = pal_obp0[pixel];}
          else {color = pal_obp1[pixel];}
          Uint32 * pixels = (Uint32 *)screen->pixels;
          pixels[((mem[_OAM + i*4]+j) * screen->w) + k] = color;
        }
      }
    }
  }
}

void step_gpu(void)
{
  switch(mode)
  {
    //reading OAM
    case 0x02:
    {
      if (m_clk >= 80)
      {
        m_clk = 0;
        mode = 3;
      }
      break;
    }
    //reading VRAM
    case 0x03:
    {
      if (m_clk >= 172)
      {
        m_clk = 0;
        mode = 0;
        draw_screen();
      }
      break;
    }
    //H blank
    case 0x00:
    {
      if (m_clk >= 204)
      {
        m_clk = 0;
        line++;
        if (line == 143)
        {
          mode = 1;
          SDL_Flip(screen);
        }
        else {mode = 2;}
      }
      break;
    }
    //V blank
    case 0x01:
    {
      if (m_clk >= 456)
      {
        m_clk = 0;
        line++;
        if (line > 153)
        {
          mode = 2;
          line = 0;
        }
      }
      break;
    }
  }
}