#ifndef KEYPAD_H
#define KEYPAD_H
#include <SDL2/SDL.h>
#include "mem.h"

using namespace std;

enum request {none, resize, quit};

class keypad {
  public:
    request handle_events(mem &m, double *sleep_factor);
  private:
    SDL_Event event;
};

#endif
