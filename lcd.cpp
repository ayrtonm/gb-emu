#include <iostream>
#include <unistd.h>
#include "lcd.h"
#include "mem.h"
#define RENDERING
#define LCDUPDATECLK 10000

lcd::lcd()
{
  scale = 1;
  clk = 0;
  screenupdateclk = 0;
  SDL_Init(SDL_INIT_EVERYTHING);
  pixels.resize(160*144*4);
#ifdef RENDERING
  window = SDL_CreateWindow("Game Boy Emulator",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,160,144,SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
  offset.x = 0;
  offset.y = 0;
  offset.w = 160*scale;
  offset.h = 144*scale;
#endif
  cout << "lcd initialized\n";
}
lcd::~lcd()
{
#ifdef RENDERING
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
#endif
  SDL_Quit();
}

void lcd::step_lcd(int dt, mem &m)
{
#ifdef DEBUG
  cout << hex << (int) (m.read_byte(O_IO+IO_LCDSTAT) & 0x03) << "\n";
  cout << "on line " << (int) m.read_byte(O_IO+IO_LY) << "\n";
#endif
  screenupdateclk += dt;
  clk -= dt;
  if (clk <= 0)
  {
    switch(m.read_byte(O_IO+IO_LCDSTAT) & 0x03)
    {
      //Horizontal Blank
      case 0x00:
      {
        m.write_byte(O_IO+IO_LY, m.read_byte(O_IO+IO_LY)+1);
        compareLYtoLYC(m);
        if (m.read_byte(O_IO+IO_LY) < 143)
        {
          //set lcd mode to OAM
          m.write_byte(O_IO+IO_LCDSTAT, (m.read_byte(O_IO+IO_LCDSTAT) | 0x02) & ~(0x01));
          clk = T_OAM;
        }
        else
        {
          //request vblank interrupt
          m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_VBL);
          //set lcd mode to VBLANK 
          m.write_byte(O_IO+IO_LCDSTAT, (m.read_byte(O_IO+IO_LCDSTAT) | 0x01) & ~(0x02));
          clk = TIO_LY_INC;
        }
        break;
      }
      //Vertical Blank
      case 0x01:
      {
#ifdef RENDERING
        if (screenupdateclk >= LCDUPDATECLK) {
          SDL_UpdateTexture(screen, NULL, &pixels[0], 160*4);
          SDL_RenderCopy(renderer, screen, NULL, &offset);
          SDL_RenderPresent(renderer);
          screenupdateclk -= LCDUPDATECLK;
        }
#endif
        compareLYtoLYC(m);
        if (m.read_byte(O_IO+IO_LY) < 153)
        {
          m.write_byte(O_IO+IO_LY, m.read_byte(O_IO+IO_LY)+1);
          clk = TIO_LY_INC;
        }
        else
        {
          m.write_byte(O_IO+IO_LY, 0);
          //set lcd mode to OAM
          m.write_byte(O_IO+IO_LCDSTAT, (m.read_byte(O_IO+IO_LCDSTAT) | 0x02) & ~(0x01));
          clk = T_OAM;
        }
        break;
      }
      //OAM
      case 0x02:
      {
        //set lcd mode to VRAM 
        m.write_byte(O_IO+IO_LCDSTAT, m.read_byte(O_IO+IO_LCDSTAT) | 0x03);
        clk = T_VRAM;
        break;
      }
      case 0x03:
      {
        compareLYtoLYC(m);
        draw_line(m);
        if (m.read_byte(O_IO+IO_LCDC) & LCDC_ENABLE)
        {
          color pal;
          for (int i = 0; i < 160; i++)
          {
            if (!(m.read_byte(O_IO+IO_LCDC) & (LCDC_WIN_ENABLE|LCDC_BG_ENABLE)))
            {
              pal = {SDL_ALPHA_OPAQUE,0xc0, 0xc0, 0xc0};
            }
            else if ((m.read_byte(O_IO+IO_LCDC) & LCDC_BG_ENABLE) && !(LCDC_WIN_ENABLE & m.read_byte(O_IO+IO_LCDC)))
            {
              pal = m.get_palette(2)[linebuffer[i] & 0x03]; 
            }
            else if (!(m.read_byte(O_IO+IO_LCDC) & LCDC_BG_ENABLE) && (LCDC_WIN_ENABLE & m.read_byte(O_IO+IO_LCDC)))
            {
              pal = m.get_palette(2)[linebuffer[i] >> 2];
            }
            else 
            {
              if (!(linebuffer[i] >> 2))
              {
                pal = m.get_palette(2)[linebuffer[i] & 0x03];
              }
              else
              {
                pal = m.get_palette(2)[linebuffer[i] >> 2];
              }
            }
            pixels[(m.read_byte(O_IO+IO_LY)*160 + i)*4] = pal.b;
            pixels[(m.read_byte(O_IO+IO_LY)*160 + i)*4 + 1] = pal.g;
            pixels[(m.read_byte(O_IO+IO_LY)*160 + i)*4 + 2] = pal.r;
            pixels[(m.read_byte(O_IO+IO_LY)*160 + i)*4 + 3] = pal.a;
          }
        }
        draw_sprites(m);
        //set lcd mode to HBLANK
        m.write_byte(O_IO+IO_LCDSTAT, m.read_byte(O_IO+IO_LCDSTAT) & ~(0x03));
        clk = T_HBLANK;
        break;
      }
    }
  }
}

int lcd::parse_events(mem &m)
{
  while(SDL_PollEvent (&event))
  {
    if (event.type == SDL_WINDOWEVENT) {
      switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          if (event.window.data1 != 0 && event.window.data2 != 0)
          {
            int w, h;
            SDL_GetWindowSize(window, &w, &h);
            scale = MIN((float)w/160.0,(float)h/144.0);
            offset.x = (w - (160*scale))/2;
            offset.y = (h - (144*scale))/2;
            offset.w = 160*scale;
            offset.h = 144*scale;
            SDL_RenderClear(renderer);
            SDL_UpdateTexture(screen, NULL, &pixels[0], 160*4);
            SDL_RenderCopy(renderer, screen, NULL, &offset);
            SDL_RenderPresent(renderer);
          }
          break;
        }
      }
    }
    //update memory but don't trigger interrupt on release
    else if (event.type == SDL_KEYUP) {
      switch(event.key.keysym.sym) {
        #include "keyup.h"
      }
    }
    //update memory and trigger on interrupt
    else if (event.type == SDL_KEYDOWN) {
      switch(event.key.keysym.sym) {
        #include "keydown.h"
      }
    }
  }
  return 1;
}
void lcd::compareLYtoLYC(mem &m)
{
  if (m.read_byte(O_IO+IO_LY) == m.read_byte(O_IO+IO_LYC))
  {
    m.write_byte(O_IO+IO_LCDSTAT, m.read_byte(O_IO+IO_LCDSTAT) | 0x04);
    if (m.read_byte(O_IO+IO_LCDSTAT) & 0x40) 
    {
      m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_LCD);
    }
  }
  else
  {
    m.write_byte(O_IO+IO_LCDSTAT, m.read_byte(O_IO+IO_LCDSTAT) & ~(0x04));
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
  if (m.read_byte(O_IO+IO_LCDC) & LCDC_BG_ENABLE)
  {
    uint16 mapoffset = ((((m.read_byte(O_IO+IO_LY) + m.read_byte(O_IO+IO_SCY)) >> 3) & 31) << 5) + ((m.read_byte(O_IO+IO_SCX) >> 3) & 31);
    uint8 t_map_number = ((m.read_byte(O_IO+IO_LCDC) & LCDC_BG_MAP) ? m.read_byte(O_VRAM + mapoffset + V_MD_1) : m.read_byte(O_VRAM + mapoffset + V_MD_0));
    if (!(m.read_byte(O_IO+IO_LCDC) & LCDC_BG_DATA))
    {
      if (t_map_number > 127) {t_map_number -= 128;}
      else {t_map_number += 128;}
    }
    uint8 x = m.read_byte(O_IO+IO_SCX) & 7;
    uint8 y = (m.read_byte(O_IO+IO_LY) + m.read_byte(O_IO+IO_SCY)) & 7;
    uint16 t_data = ((m.read_byte(O_IO+IO_LCDC) & LCDC_BG_DATA) ? m.read_word(O_VRAM + 16*t_map_number + (y << 1)) : m.read_word(O_VRAM + 16*t_map_number + (y << 1) + V_TD_1));
    for (int i = 0; i < 160; i++)
    {
      //see comment on counting backwards in draw_sprite
      //here we count up since x is a uint8 (return type of read_byte)
      //to avoid having to depend on underflow let's count normally and use 7-x to access data
      uint8 a = (LOW(t_data) & BIT(7-x)) >> (7-x);
      uint8 b = (HIGH(t_data) & BIT(7-x)) >> (7-x);
      uint8 c = a + (b << 1);
      linebuffer[i] = c;
      x++;
      if (x == 8)
      {
        x = 0;
        mapoffset = ((((m.read_byte(O_IO+IO_LY) + m.read_byte(O_IO+IO_SCY)) >> 3) & 31) << 5) + (((m.read_byte(O_IO+IO_SCX) + i) >> 3) & 31);
        uint8 t_map_number = ((m.read_byte(O_IO+IO_LCDC) & LCDC_BG_MAP) ? m.read_byte(O_VRAM + mapoffset + V_MD_1) : m.read_byte(O_VRAM + mapoffset + V_MD_0));
        if (!(m.read_byte(O_IO+IO_LCDC) & LCDC_BG_DATA))
        {
          if (t_map_number > 127) {t_map_number -= 128;}
          else {t_map_number += 128;}
        }
        t_data = ((m.read_byte(O_IO+IO_LCDC) & LCDC_BG_DATA) ? m.read_word(O_VRAM + 16*t_map_number + (y << 1)) : m.read_word(O_VRAM + 16*t_map_number + (y << 1) + V_TD_1));
      }
    }
  }
  else if (!(m.read_byte(O_IO+IO_LCDC) & LCDC_BG_ENABLE)) {for(int i = 0; i < 160; i++) {linebuffer[i] = 0;}}
  if (m.read_byte(O_IO+IO_LCDC) & LCDC_WIN_ENABLE && m.read_byte(O_IO+IO_WY) <= m.read_byte(O_IO+IO_LY))
  {
    uint8 w_offset = (((m.read_byte(O_IO+IO_LY) - m.read_byte(O_IO+IO_WY)) >> 3) & 31) << 5;
    uint8 w_map_number = ((m.read_byte(O_IO+IO_LCDC) & LCDC_WIN_MAP) ? m.read_byte(O_VRAM + w_offset + V_MD_1) : m.read_byte(O_VRAM + w_offset + V_MD_0));
    //offsets within tile
    int x = 0;
    uint8 y = (m.read_byte(O_IO+IO_LY) - m.read_byte(O_IO+IO_WY)) & 7;
    uint16 w_data = m.read_word(O_VRAM + 16*w_map_number + (y << 1) + V_TD_1);
    int i;
    for (i = MAX(m.read_byte(O_IO+IO_WX)-7,0); i < 160; i++)
    {
      uint8 a = (LOW(w_data) & BIT(7-x)) >> (7-x);
      uint8 b = (HIGH(w_data) & BIT(7-x)) >> (7-x);
      uint8 c =  (a << 2) + (b << 3);
      linebuffer[i] += c;
      x++;
      if (x == 8)
      {
        x = 0;
        w_offset = (((m.read_byte(O_IO+IO_LY)-m.read_byte(O_IO+IO_WY)) >> 3) & 31) << 5;
        uint8 w_map_number = ((m.read_byte(O_IO+IO_LCDC) & LCDC_WIN_MAP) ? m.read_byte(O_VRAM + w_offset + V_MD_1) : m.read_byte(O_VRAM + w_offset + V_MD_0));
        w_data = m.read_byte(O_VRAM + 16*w_map_number + (y << 1) + V_TD_1);
      }
    }
  }
}

void lcd::draw_sprites(mem &m)
{
  //OAM(s,p) = m.read_byte(O_OAM+LOW((s << 2) + p))
  if (m.read_byte(O_IO+IO_LCDC) & LCDC_OBJ_ENABLE)
  {
    int count = 0;
    for (int i = 0; i < 40; i++)
    {
      if (m.read_byte(O_OAM+(i * 4)) - 16 <= m.read_byte(O_IO+IO_LY) && (m.read_byte(O_OAM+(i * 4)) - 16 + ((m.read_byte(O_IO+IO_LCDC) & LCDC_OBJ_SIZE) ? 16 : 8)) > m.read_byte(O_IO+IO_LY))
      {
        uint8 y = ((m.read_byte(O_IO+IO_LY) - m.read_byte(O_OAM+(i * 4)) + 16) & 7) << 1;
        uint8 yflip = (7 - ((m.read_byte(O_IO+IO_LY) - m.read_byte(O_OAM+(i * 4)) + 16) & 7)) << 1;
        uint8 t_number = m.read_byte(O_OAM+((i * 4) + 2));
        uint16 t_data;
        if (!(m.read_byte(O_IO+IO_LCDC) & LCDC_OBJ_SIZE))//8x8 mode
        {
          t_data = m.read_byte(O_VRAM + 16*t_number + ((m.read_byte(O_OAM+(i * 4) + 3) & OAM_F_YFLIP) ? yflip : y));
        }
        else //8x16 mode
        {
          t_data = m.read_byte(O_VRAM + 16*(((m.read_byte(O_IO+IO_LY) - m.read_byte(O_OAM+(i * 4)) + 16) > 7) || !(yflip) ? (t_number | 0x01) : (t_number & 0xFE)) + ((m.read_byte(O_OAM+(i * 4) + 3) & OAM_F_YFLIP) ? yflip : y));
        }
        if (!(m.read_byte(O_OAM+((i * 4) + 3)) & OAM_F_XFLIP)) {REVERSE_WORD(t_data);}
        count++;
        //counting backwards since bit 7 is leftmost pixel and bit 0 is rightmost
        for (int x = 7; x >= 0; x--)
        {
          if (m.read_byte(O_OAM+((i * 4) + 1)) + x - 8 >= 0 && m.read_byte(O_OAM+((i * 4) + 1)) + x - 8 < 160 && (!(m.read_byte(O_OAM+((i * 4) + 3)) & OAM_F_BG) || (linebuffer[m.read_byte(O_OAM+((i * 4) + 1)) + x - 8] & 0x03) == 0))
          {
            uint8 a = (LOW(t_data) & BIT(x)) >> x;
            uint8 b = (HIGH(t_data) & BIT(x)) >> x;
            uint8 c = a + (b << 1);
            if (c != 0)
            {
              color pal;
              pal = (m.read_byte(O_OAM+((i * 4) + 3)) & OAM_F_PAL ? m.get_palette(1)[c] : m.get_palette(0)[c]);
              pixels[(m.read_byte(O_IO+IO_LY) * 160 + m.read_byte(O_OAM+((i * 4) + 1)) + x - 8)*4] = pal.b;
              pixels[(m.read_byte(O_IO+IO_LY) * 160 + m.read_byte(O_OAM+((i * 4) + 1)) + x - 8)*4 + 1] = pal.g;
              pixels[(m.read_byte(O_IO+IO_LY) * 160 + m.read_byte(O_OAM+((i * 4) + 1)) + x - 8)*4 + 2] = pal.r;
              pixels[(m.read_byte(O_IO+IO_LY) * 160 + m.read_byte(O_OAM+((i * 4) + 1)) + x - 8)*4 + 3] = pal.a;
            }
          }
        }
      }
      if (count >= 10) break;//can't draw more than 10 sprites
    }
  }
}
