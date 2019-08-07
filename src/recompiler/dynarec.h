#ifndef DYNAREC_H
#define DYNAREC_H
#include "../mem.h"
#include "../keypad.h"
#include "../lcd.h"
#include "../sound.h"

class dynarec_cpu {
  public:
    dynarec_cpu();
    ~dynarec_cpu();
    void emulate(mem &m, keypad &k, lcd &l, sound &s);
  private:
    word16 pc;
};

#endif
