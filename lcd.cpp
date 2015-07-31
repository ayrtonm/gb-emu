#include "globals.h"
#include <iostream>

using namespace std;

lcd::lcd()
{
  scale = 1;
  clk = 0;
  SDL_Init(SDL_INIT_EVERYTHING);
  screen = SDL_CreateRGBSurface(SDL_HWSURFACE,160,144,32,0,0,0,0);
  visible = SDL_SetVideoMode(160*scale,144*scale,32,SDL_HWSURFACE|SDL_RESIZABLE);
  offset.x = 0;
  offset.y = 0;
  SDL_WM_SetCaption("Game Boy Emulator",NULL);
  cout << "lcd initialized\n";
}
lcd::~lcd()
{
  SDL_FreeSurface(screen);
  SDL_FreeSurface(visible);
  SDL_Quit();
}

void lcd::step_lcd(uint8 dt, mem &m)
{
//  cout << hex << (int) (m.io.at(IO_LCDSTAT) & 0x03) << "\n";
//  cout << "on line " << (int) m.io.at(IO_LY) << "\n";
  clk -= dt;
  if (clk <= 0)
  {
    switch(m.io.at(IO_LCDSTAT) & 0x03)
    {
      //Horizontal Blank
      case 0x00:
      {
        m.io.at(IO_LY)++;
        compareLYtoLYC(m);
        if (m.io.at(IO_LY) < 143)
        {
          //set lcd mode to OAM
          SET(0x02,m.io.at(IO_LCDSTAT));
          CLEAR(0x01,m.io.at(IO_LCDSTAT));
          clk = T_OAM;
        }
        else
        {
          //request vblank interrupt
          SET(INT_VBL,m.io.at(IO_IR));
          //set lcd mode to VBLANK 
          SET(0x01,m.io.at(IO_LCDSTAT));
          CLEAR(0x02,m.io.at(IO_LCDSTAT));
          clk = TIO_LY_INC;
        }
        break;
      }
      //Vertical Blank
      case 0x01:
      {
        SDL_BlitSurface(zoomSurface(screen,scale,scale,0),NULL,visible,&offset);
        SDL_Flip(visible);
        compareLYtoLYC(m);
        if (m.io.at(IO_LY) < 153)
        {
          m.io.at(IO_LY)++;
          clk = TIO_LY_INC;
        }
        else
        {
          m.io.at(IO_LY) = 0;
          //set lcd mode to OAM
          SET(0x02,m.io.at(IO_LCDSTAT));
          CLEAR(0x01,m.io.at(IO_LCDSTAT));
          clk = T_OAM;
        }
        break;
      }
      //OAM
      case 0x02:
      {
        //set lcd mode to VRAM 
        SET(0x03,m.io.at(IO_LCDSTAT));
        clk = T_VRAM;
        break;
      }
      case 0x03:
      {
        compareLYtoLYC(m);
        draw_line(m);
        if (m.io.at(IO_LCDC) & LCDC_ENABLE)
        {
          Uint32 color;
          Uint32 *pixels = (Uint32 *)screen->pixels;
          for (int i = 0; i < 160; i++)
          {
            if (!(m.io.at(IO_LCDC) & (LCDC_WIN_ENABLE|LCDC_BG_ENABLE)))
            {
              color = SDL_MapRGB(screen->format,0xff,0xff,0xff);
            }
            else if ((m.io.at(IO_LCDC) & LCDC_BG_ENABLE) && !(LCDC_WIN_ENABLE & m.io.at(IO_LCDC)))
            {
              color = m.pal_bgp[linebuffer[i] & 0x03]; 
            }
            else if (!(m.io.at(IO_LCDC) & LCDC_BG_ENABLE) && (LCDC_WIN_ENABLE & m.io.at(IO_LCDC)))
            {
              color = m.pal_bgp[linebuffer[i] >> 2];
            }
            else 
            {
              if (!(linebuffer[i] >> 2))
              {
                color = m.pal_bgp[linebuffer[i] & 0x03];
              }
              else
              {
                color = m.pal_bgp[linebuffer[i] >> 2];
              }
            }
            pixels[(m.io.at(IO_LY)*screen->w) + i] = color;
          }
        }
        draw_sprites(m);
        //set lcd mode to HBLANK
        CLEAR(0x03,m.io.at(IO_LCDSTAT));
        clk = T_HBLANK;
        break;
      }
    }
  }
}

int lcd::parse_events(void)
{
  while(SDL_PollEvent (&event))
  {
    switch(event.type)
    {
      case SDL_VIDEORESIZE:
      {
        if (event.resize.w != 0 && event.resize.h != 0)
        {
          visible = SDL_SetVideoMode(MAX(160,event.resize.w),MAX(144,event.resize.h),32,SDL_HWSURFACE|SDL_RESIZABLE);
          scale = MIN((float)visible->w/160.0,(float)visible->h/144.0);
          offset.x = (visible->w - (160*scale))/2;
          offset.y = (visible->h - (144*scale))/2;
          SDL_BlitSurface(zoomSurface(screen,scale,scale,0),NULL,visible,&offset);
        }
      }
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_q: return 0;
        }
      }
    }
  }
  return 1;
}
void lcd::compareLYtoLYC(mem &m)
{
  if (m.io.at(IO_LY) == m.io.at(IO_LYC))
  {
    SET(0x04,m.io.at(IO_LCDSTAT));
    if (m.io.at(IO_LCDSTAT) & 0x40) 
    {
      SET(INT_LCD,m.io.at(IO_IR));
    }
  }
  else
  {
    CLEAR(0x04,m.io.at(IO_LCDSTAT));
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
void lcd::draw_line(mem &m)
{
  if (m.io.at(IO_LCDC) & LCDC_BG_ENABLE)
  {
    uint16 mapoffset = ((((m.io.at(IO_LY) + m.io.at(IO_SCY)) >> 3) & 31) << 5) + ((m.io.at(IO_SCX) >> 3) & 31);
    uint8 t_map_number = ((m.io.at(IO_LCDC) & LCDC_BG_MAP) ? m.vram.at(mapoffset + V_MD_1) : m.vram.at(mapoffset + V_MD_0));
    if (!(m.io.at(IO_LCDC) & LCDC_BG_DATA))
    {
      if (t_map_number > 127) {t_map_number -= 128;}
      else {t_map_number += 128;}
    }
    uint8 x = m.io.at(IO_SCX) & 7;
    uint8 y = (m.io.at(IO_LY) + m.io.at(IO_SCY)) & 7;
    uint16 t_data = ((m.io.at(IO_LCDC) & LCDC_BG_DATA) ? m.vram.at(16*t_map_number + (y << 1)) : m.vram.at(16*t_map_number + (y << 1) + V_TD_1));
    for (int i = 0; i < 160; i++)
    {
      uint8 a = (LOW(t_data) & BIT(x)) >> x;
      uint8 b = (HIGH(t_data) & BIT(x)) >> x;
      uint8 c = a + (b << 1);
      linebuffer.at(i) = c;
      x++;
      if (x == 8)
      {
        x = 0;
        mapoffset = ((((m.io.at(IO_LY) + m.io.at(IO_SCY)) >> 3) & 31) << 5) + (((m.io.at(IO_SCX) + i) >> 3) & 31);
        uint8 t_map_number = ((m.io.at(IO_LCDC) & LCDC_BG_MAP) ? m.vram.at(mapoffset + V_MD_1) : m.vram.at(mapoffset + V_MD_0));
        if (!(m.io.at(IO_LCDC) & LCDC_BG_DATA))
        {
          if (t_map_number > 127) {t_map_number -= 128;}
          else {t_map_number += 128;}
        }
        uint16 t_data = ((m.io.at(IO_LCDC) & LCDC_BG_DATA) ? m.vram.at(16*t_map_number + (y << 1)) : m.vram.at(16*t_map_number + (y << 1) + V_TD_1));
      }
    }
  }
  else if (!(m.io.at(IO_LCDC) & LCDC_BG_ENABLE)) {for(int i = 0; i < 160; i++) {linebuffer.at(i) = 0;}}
  if (m.io.at(IO_LCDC) & LCDC_WIN_ENABLE && m.io.at(IO_WY) <= m.io.at(IO_LY))
  {
    uint8 w_offset = (((m.io.at(IO_LY) - m.io.at(IO_WY)) >> 3) & 31) << 5;
    uint8 w_map_number = ((m.io.at(IO_LCDC) & LCDC_WIN_MAP) ? m.vram.at(w_offset + V_MD_1) : m.vram.at(w_offset + V_MD_0));
    //offsets within tile
    int x = 0;
    uint8 y = (m.io.at(IO_LY) - m.io.at(IO_WY)) & 7;
    uint16 w_data = m.vram.at(16*w_map_number + (y << 1) + V_TD_1);
    int i;
    for (i = MAX(m.io.at(IO_WX)-7,0); i < 160; i++)
    {
      uint8 a = (LOW(w_data) & BIT(x)) >> x;
      uint8 b = (HIGH(w_data) & BIT(x)) >> x;
      uint8 c =  (a << 2) + (b << 3);
      linebuffer.at(i) += c;
      x++;
      if (x == 8)
      {
        x = 0;
        w_offset = (((m.io.at(IO_LY)-m.io.at(IO_WY)) >> 3) & 31) << 5;
        uint8 w_map_number = ((m.io.at(IO_LCDC) & LCDC_WIN_MAP) ? m.vram.at(w_offset + V_MD_1) : m.vram.at(w_offset + V_MD_0));
        uint16 w_data = m.vram.at(16*w_map_number + (y << 1) + V_TD_1);
      }
    }
  }
}

void lcd::draw_sprites(mem &m)
{
  //OAM(s,p) = m.oam.at(LOW((s << 2) + p))
  if (m.io.at(IO_LCDC) & LCDC_OBJ_ENABLE)
  {
    int count = 0;
    for (int i = 0; i < 40; i++)
    {
      if (m.oam.at(LOW(i << 2)) - 16 <= m.io.at(IO_LY) && (m.oam.at(LOW(i << 2)) - 16 + ((m.io.at(IO_LCDC) & LCDC_OBJ_SIZE) ? 16 : 8)) > m.io.at(IO_LY))
      {
        uint8 y = ((m.io.at(IO_LY) - m.oam.at(LOW(i << 2)) + 16) & 7) << 1;
        uint8 yflip = (7 - ((m.io.at(IO_LY) - m.oam.at(LOW(i << 2)) + 16) & 7)) << 1;
        uint8 t_number = m.oam.at(LOW((i << 2) + 2));
        uint16 t_data;
        if (!(m.io.at(IO_LCDC) & LCDC_OBJ_SIZE))//8x8 mode
        {
          t_data = m.vram.at(16*t_number + ((m.oam.at(LOW((i << 2) + 3)) & OAM_F_YFLIP) ? yflip : y));
        }
        else //8x16 mode
        {
          t_data = m.vram.at(16*(((m.io.at(IO_LY) - m.oam.at(LOW(i << 2)) + 16) > 7) || !(yflip) ? (t_number | 0x01) : (t_number & 0xFE)) + ((m.oam.at(LOW((i << 2) + 3) & OAM_F_YFLIP) ? yflip : y)));
        }
        if (!(m.oam.at(LOW((i << 2) + 3)) & OAM_F_XFLIP)) {REVERSE_WORD(t_data);}
        count++;
        for (int x = 0; x < 8; x++)
        {
          if (m.oam.at(LOW((i << 2) + 1)) + x - 8 >= 0 && m.oam.at(LOW((i << 2) + 1)) + x - 8 < 160 && (!(m.oam.at(LOW((i << 2) + 3)) & OAM_F_BG) || (linebuffer.at(m.oam.at(LOW((i << 2) + 1)) + x - 8) & 0x03) == 0))
          {
            uint8 a = (LOW(t_data) & BIT(x)) >> x;
            uint8 b = (HIGH(t_data) & BIT(x)) >> x;
            uint8 c = a + (b << 1);
            if (c != 0)
            {
              Uint32 color;
              Uint32 *pixels = (Uint32 *)screen->pixels;
              color = (m.oam.at(LOW((i << 2) + 3)) & OAM_F_PAL ? m.pal_obp1[c] : m.pal_obp0[c]);
              pixels[m.io.at(IO_LY) * screen->w + m.oam.at(LOW((i << 2) + 1)) + x - 8] = color;
            }
          }
        }
      }
      if (count >= 10) break;//can't draw more than 10 sprites
    }
  }
}
