#ifndef LCD_H
#define LCD_H
#include "bits.h"
#include "mem.h"
#include <SDL/SDL.h>
#include <array>

using namespace std;

class lcd
{
  public:
    lcd();
    ~lcd();
    SDL_Surface *screen, *visible;
    SDL_Rect offset;
    SDL_Event event;
    float scale;
    array<uint8,160> linebuffer;
    int clk;
    void step_lcd(uint8 dt, mem &m);
    void compareLYtoLYC(mem &m);
    void draw_line(mem &m);

    uint16 reverse_word(uint16 input);
    void draw_sprites(mem &m);
    int parse_events(mem &m);
    SDL_Surface *zoomSurface (SDL_Surface * src, double zoomx, double zoomy, int smooth);
};

#endif
