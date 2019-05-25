#ifndef LCD_H
#define LCD_H
#include "bits.h"
#include "mem.h"
#include <SDL2/SDL.h>
#include <array>
#include <vector>

using namespace std;

class lcd
{
  public:
    lcd();
    ~lcd();

    void step_lcd(int dt, mem &m);
    void compareLYtoLYC(mem &m);
    void draw_line(mem &m);
    void draw_sprites(mem &m);

  private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *screen;
    SDL_Rect offset;

    float scale;
    array<uint8,160> linebuffer;
    vector<uint8> pixels;
    int clk, screenupdateclk;

};

#endif
