#include <iostream>
#include <fstream>
#include <algorithm>
#include "lcd.h"
#include "mem.h"
#define RENDERING
#define DRAWSPRITES
#define DRAWBG
#define DRAWWIN
#define LCDUPDATECLK 20000

using namespace std;

lcd::lcd(string configfile) {
  if (configfile != "") {
    ifstream config;
    try {
      config.open(configfile, ifstream::in);
      if (!config.is_open()) {
        throw runtime_error("unable to open config file");
        return;
      }
      string option, value;
      while (config >> option >> value) {
        if (option == "lcd.x") {
          xinit = stoi(value);
          cout << "setting " << option << " to " << xinit << endl;
        }
        else if (option == "lcd.y") {
          yinit = stoi(value);
          cout << "setting " << option << " to " << yinit << endl;
        }
        else if (option == "lcd.resizable") {
          resizable = (value == "true" ? true : false);
          cout << "setting " << option << " to " << (resizable ? "true" : "false") << endl;
        }
        else if (option == "lcd.fullscreen") {
          fullscreen = (value == "true" ? true : false);
          cout << "setting " << option << " to " << (fullscreen ? "true" : "false") << endl;
        }
      }
      config.close();
    }
    catch (const ifstream::failure &e) {
      cout << e.what() << endl;
      exit(3);
    }
  }
  bgbuffer.fill(0);
  scale = 1;
  clk = 0;
  screenupdateclk = 0;
  SDL_Init(SDL_INIT_EVERYTHING);
#ifdef RENDERING
  if (resizable) {
    window = SDL_CreateWindow("Game Boy Emulator",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,xinit,yinit,SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);
  }
  else if (fullscreen) {
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    window = SDL_CreateWindow("Game Boy Emulator",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,DM.w,DM.h,SDL_WINDOW_SHOWN|SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  else {
    window = SDL_CreateWindow("Game Boy Emulator",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,xinit,yinit,SDL_WINDOW_SHOWN);
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
  offset.x = 0;
  offset.y = 0;
  offset.w = xinit*scale;
  offset.h = yinit*scale;
  this->resize();
#endif
}
lcd::~lcd() {
#ifdef RENDERING
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
#endif
  SDL_Quit();
}

void lcd::resize() {
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

void lcd::step_lcd(int dt, mem &m) {
  screenupdateclk += dt;
  clk -= dt;
  if (clk <= 0) {
    switch(m.read_byte_internal(O_IO+IO_LCDSTAT) & 0x03) {
      //Horizontal Blank
      case 0x00: {
        m.write_byte_internal(O_IO+IO_LY, m.read_byte_internal(O_IO+IO_LY)+1);
        compareLYtoLYC(m);
        if (m.read_byte_internal(O_IO+IO_LY) < 144) {
          //if OAM interrupt enabled
          if (m.read_byte_internal(O_IO+IO_LCDSTAT) & LCDSTAT_OAM_INT) {
            //request OAM interrupt
            m.write_byte_internal(O_IO+IO_IR, m.read_byte_internal(O_IO+IO_IR) | INT_LCD);
          }
          //set lcd mode to OAM
          m.write_byte_internal(O_IO+IO_LCDSTAT, (m.read_byte_internal(O_IO+IO_LCDSTAT) | 0x02) & ~(0x01));
          clk += T_OAM;
        }
        else {
          //not sure which interrupt to request when switching to vblank
          //if vblank interrupt enabled
          if (m.read_byte_internal(O_IO+IO_LCDSTAT) & LCDSTAT_VBL_INT) {
            //request vblank interrupt
            m.write_byte_internal(O_IO+IO_IR, m.read_byte_internal(O_IO+IO_IR) | INT_LCD);
          }
          //request vblank interrupt
          m.write_byte_internal(O_IO+IO_IR, m.read_byte_internal(O_IO+IO_IR) | INT_VBL);
          //set lcd mode to VBLANK 
          m.write_byte_internal(O_IO+IO_LCDSTAT, (m.read_byte_internal(O_IO+IO_LCDSTAT) | 0x01) & ~(0x02));
          clk += TIO_LY_INC;
        }
        break;
      }
      //Vertical Blank
      case 0x01: {
#ifdef RENDERING
        if (screenupdateclk >= LCDUPDATECLK) {
          SDL_UpdateTexture(screen, NULL, &pixels[0], 160*4);
          SDL_RenderCopy(renderer, screen, NULL, &offset);
          SDL_RenderPresent(renderer);
          screenupdateclk -= LCDUPDATECLK;
        }
#endif
        if (m.read_byte_internal(O_IO+IO_LY) < 154) {
          m.write_byte_internal(O_IO+IO_LY, m.read_byte_internal(O_IO+IO_LY)+1);
          compareLYtoLYC(m);
          clk += TIO_LY_INC;
        }
        else {
          m.write_byte_internal(O_IO+IO_LY, 0);
          compareLYtoLYC(m);
          //if OAM interrupt enabled
          if (m.read_byte_internal(O_IO+IO_LCDSTAT) & LCDSTAT_OAM_INT) {
            //request OAM interrupt
            m.write_byte_internal(O_IO+IO_IR, m.read_byte_internal(O_IO+IO_IR) | INT_LCD);
          }
          //set lcd mode to OAM
          m.write_byte_internal(O_IO+IO_LCDSTAT, (m.read_byte_internal(O_IO+IO_LCDSTAT) | 0x02) & ~(0x01));
          clk += T_OAM;
        }
        break;
      }
      //OAM
      case 0x02: {
        //set lcd mode to VRAM 
        m.write_byte_internal(O_IO+IO_LCDSTAT, m.read_byte_internal(O_IO+IO_LCDSTAT) | 0x03);
        clk += T_VRAM;
        break;
      }
      case 0x03: {
        compareLYtoLYC(m);
        if (m.read_byte_internal(O_IO+IO_LCDC) & LCDC_ENABLE) {
          //background and window both off
          if (!(m.read_byte_internal(O_IO+IO_LCDC) & (LCDC_WIN_ENABLE|LCDC_BG_ENABLE))) {
            auto linestart = m.read_byte_internal(O_IO+IO_LY)*160;
            fill(pixels.begin()+linestart, pixels.begin()+linestart+160,(color){SDL_ALPHA_OPAQUE,0xc0,0xc0,0xc0});
          }

          //background on, window off
          else if ((m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_ENABLE) && !(LCDC_WIN_ENABLE & m.read_byte_internal(O_IO+IO_LCDC))) {
            draw_bg(m);
          }

          //background off, window on
          else if (!(m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_ENABLE) && (LCDC_WIN_ENABLE & m.read_byte_internal(O_IO+IO_LCDC))) {
            fill(bgbuffer.begin(), bgbuffer.end(), 0);
            draw_win(m);
          }

          //background and window both on
          else {
            draw_bg(m);
            draw_win(m);
          }
        }
        draw_sprites(m);
        //if hblank interrupt enabled
          if (m.read_byte_internal(O_IO+IO_LCDSTAT) & LCDSTAT_HBL_INT) {
            //request HBL interrupt
            m.write_byte_internal(O_IO+IO_IR, m.read_byte_internal(O_IO+IO_IR) | INT_LCD);
          }
        //set lcd mode to HBLANK
        m.write_byte_internal(O_IO+IO_LCDSTAT, m.read_byte_internal(O_IO+IO_LCDSTAT) & ~(0x03));
        clk += T_HBLANK;
        break;
      }
    }
  }
}

void lcd::compareLYtoLYC(mem &m) {
  uint8 curline = m.read_byte_internal(O_IO+IO_LY);
  uint8 compline = m.read_byte_internal(O_IO+IO_LYC);
  if (curline == compline) {
    m.write_byte_internal(O_IO+IO_LCDSTAT, m.read_byte_internal(O_IO+IO_LCDSTAT) | 0x04);
    if (m.read_byte_internal(O_IO+IO_LCDSTAT) & LCDSTAT_LYC_INT) {
      m.write_byte_internal(O_IO+IO_IR, m.read_byte_internal(O_IO+IO_IR) | INT_LCD);
    }
  }
  else {
    m.write_byte_internal(O_IO+IO_LCDSTAT, m.read_byte_internal(O_IO+IO_LCDSTAT) & ~(0x04));
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
void lcd::draw_bg(mem &m) {
#ifdef DRAWBG
  array<color,4> pal = m.get_palette(2);
  uint8 curline = m.read_byte_internal(O_IO+IO_LY);
  uint8 scrolly = m.read_byte_internal(O_IO+IO_SCY);
  uint8 scrollx = m.read_byte_internal(O_IO+IO_SCX);
  uint16 mapoffset = 32*(((curline + scrolly) >> 3) & 31) + ((scrollx >> 3) & 31);
  uint8 t_map_number;
  if (m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_MAP) {
    t_map_number = m.read_byte_internal(O_VRAM + mapoffset + V_MD_1);
  }
  else {
    t_map_number = m.read_byte_internal(O_VRAM + mapoffset + V_MD_0);
  }
  if (!(m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_DATA)) {
    if (t_map_number > 127) {t_map_number -= 128;}
    else {t_map_number += 128;}
  }
  uint8 x = scrollx & 7;
  uint8 y = (curline + scrolly) & 7;
  uint16 t_data;
  //each tile is 16 bytes
  //each line in a tile is 2 bytes
  if (m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_DATA) {
    t_data = m.read_word_internal(O_VRAM + 16*t_map_number + 2*y);
  }
  else {
    t_data = m.read_word_internal(O_VRAM + 16*t_map_number + 2*y + V_TD_1);
  }
  for (int i = 0; i < 160; i++) {
    //see comment on counting backwards in draw_sprite
    //here we count up since x is a uint8 (return type of read_byte_internal)
    //to avoid having to depend on underflow let's count normally and use 7-x to access data
    uint8 a = (LOW(t_data) & BIT(7-x)) >> (7-x);
    uint8 b = (HIGH(t_data) & BIT(7-x)) >> (7-x);
    uint8 c = a + (b << 1);
    bgbuffer[i] = c;
    pixels[(curline * 160) + i] = pal[c];
    x++;
    if (x == 8) {
      x = 0;
      mapoffset = 32*(((curline + scrolly) >> 3) & 31) + (((scrollx + i + 1) >> 3) & 31);
      if (m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_MAP) {
        t_map_number = m.read_byte_internal(O_VRAM + mapoffset + V_MD_1);
      }
      else {
        t_map_number = m.read_byte_internal(O_VRAM + mapoffset + V_MD_0);
      }
      if (!(m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_DATA)) {
        if (t_map_number > 127) {t_map_number -= 128;}
        else {t_map_number += 128;}
      }
      if (m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_DATA) {
        t_data = m.read_word_internal(O_VRAM + 16*t_map_number + 2*y);
      }
      else {
        t_data = m.read_word_internal(O_VRAM + 16*t_map_number + 2*y + V_TD_1);
      }
    }
  }
#endif
}

void lcd::draw_win(mem &m) {
#ifdef DRAWWIN
  array<color,4> pal = m.get_palette(2);
  uint8 curline = m.read_byte_internal(O_IO+IO_LY);
  uint8 winy = m.read_byte_internal(O_IO+IO_WY);
  uint8 winx = m.read_byte_internal(O_IO+IO_WX);
  if ((winy <= curline) && (winx < 167) && (winy < 144)) {
    uint16 w_offset = 32*((curline-winy) >> 3);
    uint8 w_map_number;
    if (m.read_byte(O_IO+IO_LCDC) & LCDC_WIN_MAP) {
      w_map_number = m.read_byte_internal(O_VRAM + w_offset + V_MD_1);
    }
    else {
      w_map_number = m.read_byte_internal(O_VRAM + w_offset + V_MD_0);
    }
    if ((m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_DATA) == 0) {
      if (w_map_number > 127) {w_map_number -= 128;}
      else {w_map_number += 128;}
    }
    //offsets within tile
    uint8 x = max(7 - winx, 0) & 7;
    uint8 y = (curline - winy) & 7;
    uint16 w_data = m.read_word_internal(O_VRAM + 16*w_map_number + 2*y + V_TD_1);
    for (int i = max(winx-7,0); i < 160; i++) {
      uint8 a = (LOW(w_data) & BIT(7-x)) >> (7-x);
      uint8 b = (HIGH(w_data) & BIT(7-x)) >> (7-x);
      uint8 c =  a + (b << 1);
      pixels[(curline * 160) + i] = pal[c];
      x++;
      if (x == 8) {
        x = 0;
        w_offset++;
        if (m.read_byte_internal(O_IO+IO_LCDC) & LCDC_WIN_MAP) {
          w_map_number = m.read_byte_internal(O_VRAM + w_offset + V_MD_1);
        }
        else {
          w_map_number = m.read_byte_internal(O_VRAM + w_offset + V_MD_0);
        }
        if ((m.read_byte_internal(O_IO+IO_LCDC) & LCDC_BG_DATA) == 0) {
          if (w_map_number > 127) {w_map_number -= 128;}
          else {w_map_number += 128;}
        }
        w_data = m.read_word_internal(O_VRAM + 16*w_map_number + 2*y + V_TD_1);
      }
    }
  }
#endif
}

void lcd::draw_sprites(mem &m) {
#ifdef DRAWSPRITES
  //if sprites enabled in LCDC
  if (m.read_byte_internal(O_IO+IO_LCDC) & LCDC_OBJ_ENABLE) {
    int count = 0;
    //loop through the 40 sprites in the OAM table
    uint8 curline = m.read_byte_internal(O_IO + IO_LY);
    for (int i = 0; i < 40; i++) {
      int oam_y = m.read_byte_internal(O_OAM + (i * 4)) - 16;
      int oam_x = m.read_byte_internal(O_OAM + (i * 4) + 1) - 8;
      //if part of the sprite is on the line we are current drawing LY
      if ((oam_y <= curline) && ((oam_y + (((m.read_byte_internal(O_IO+IO_LCDC) & LCDC_OBJ_SIZE) ? 16 : 8))) > curline)) {
        //get sprite properties from OAM table
        uint8 oam_prop = m.read_byte_internal(O_OAM + (i * 4) + 3);
        //get y offset within the 8x8 or 8x16 tile from the OAM table
        uint8 y = ((curline - oam_y) & 7);
        uint8 yflip = (7 - ((curline - oam_y) & 7));
        //get the tile number from the OAM table
        uint8 t_number = m.read_byte_internal(O_OAM+((i * 4) + 2));
        //if in 8x16 mode, offset the tile number
        if ((m.read_byte_internal(O_IO+IO_LCDC) & LCDC_OBJ_SIZE)) {
          bool in_lower_half = (curline - oam_y) > 7;
          bool flipped = ((oam_prop & OAM_F_YFLIP) == 0 ? false : true);
          //not equal acts as a logical exclusive or
          t_number = ((in_lower_half != flipped) ? (t_number | 0x01) : (t_number & 0xFE));
        }
        //get the 2 bytes for the sprite's current line
        uint16 t_data = m.read_word_internal(O_VRAM + 16*t_number + 2*((oam_prop & OAM_F_YFLIP) ? yflip : y));
        //if flipped in the x direction reverse the 2 bytes
        if (oam_prop & OAM_F_XFLIP) {REVERSE_BYTES(t_data);}
        count++;
        for (int x = 0; x < 8; x++) {
          //for each pixel if x coordinate is on screen (between 0 and 160) and (sprites have priority over the background or the background is clear)
          if (((oam_x + x > 0) && (oam_x + x <= 168)) && (!(oam_prop & OAM_F_BG) || (bgbuffer[oam_x + x] == 0))) {
            //accessing bits backwards since bit 7 is leftmost pixel and bit 0 is rightmost
            uint8 a = (LOW(t_data) & BIT(7-x)) >> (7-x);
            uint8 b = (HIGH(t_data) & BIT(7-x)) >> (7-x);
            uint8 c = a + (b << 1);
            if (c != 0) {
              pixels[(curline * 160) + oam_x + x] = (oam_prop & OAM_F_PAL ? m.get_palette(1)[c] : m.get_palette(0)[c]);
            }
          }
        }
      }
      if (count == 10) {
        return;//can't draw more than 10 sprites per line
      }
    }
  }
#endif
}
