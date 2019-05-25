#ifndef KEYPAD_H
#define KEYPAD_H
#include <SDL2/SDL.h>
#include "mem.h"

using namespace std;

class keypad {
  public:
    int handle_events(mem &m);
  private:
    SDL_Event event;
};

#endif
