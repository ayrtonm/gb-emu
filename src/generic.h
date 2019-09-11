#ifndef GENERIC_H
#define GENERIC_H
#include "bits.h"
#include "states.h"
#include "throttle.h"
#include "mem.h"
#include "lcd.h"
#include "keypad.h"
#include "sound.h"

class generic_cpu {
  public:
    void save_state(emulator_state *st, mem &m, keypad &k, lcd &l, sound &s);
    void load_state(emulator_state *st, mem &m, keypad &k, lcd &l, sound &s);
    void delete_state(emulator_state *st);
  protected:
    //note that dynarec currently does not use pc
    //while the dynamic recompiler does not need to keep track of pc while executing a block, it should make use of pc in between blocks instead of making various local address variables in emulate()
    word16 af, bc, de, hl, sp, pc;
    uint8 ime, ei_delay, halt;
    throttle_controller tp;
};

#endif
