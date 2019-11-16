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
    lcd(string configfile);
    ~lcd();

    void step_lcd(int dt, mem &m);
    void compareLYtoLYC(mem &m);
    void draw_bg(mem &m);
    void draw_win(mem &m);
    void draw_sprites(mem &m);
    void resize();

  private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *screen;
    SDL_Rect offset;

    string lcdoption = "lcd";
    int xinit = 160;
    int yinit = 144;
    bool resizable = true;
    bool fullscreen = false;

    float scale;
    array<uint8_t,160> bgbuffer;
    array<color,160*144> pixels;
    int clk, screenupdateclk;
};

#endif
