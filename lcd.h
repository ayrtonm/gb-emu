#ifndef LCD_H
#define LCD_H
#include "bits.h"
#include "mem.h"
#include <SDL2/SDL.h>
#include <array>

using namespace std;

class lcd
{
  public:
    lcd();
    ~lcd();
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *screen;
    SDL_Rect offset;
    SDL_Event event;
    float scale;
    array<uint8,160> linebuffer;
    vector<uint8> pixels;
    int clk, screenupdateclk;
    void step_lcd(int dt, mem &m);
    void compareLYtoLYC(mem &m);
    void draw_line(mem &m);

    uint16 reverse_word(uint16 input);
    void draw_sprites(mem &m);
    int parse_events(mem &m);
};

#endif
