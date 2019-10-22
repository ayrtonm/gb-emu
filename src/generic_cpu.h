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
    word16 af, bc, de, hl, sp, pc;
    uint8 ime, ei_delay, halt;
    throttle_controller tp;
};

//these macros should only be used outside JIT code
#define JR(n) do {pc.w += ((signed char)n);} while(0)

#define COND_JR(cond,n) do {if (cond) {JR(n);/*dt += 1;*/};} while(0)

#define RET do {POP(pc.w);} while(0)

#define COND_RET(cond) do {if (cond) {RET;/*dt += 3;*/};} while(0)

#define RETI do {POP(pc.w);ime=1;} while(0)

#define JP(n) do {pc.w = n;} while(0)

#define COND_JP(cond,n) do {if (cond) {JP(n);/*dt += 1;*/};} while(0)

#define CALL(n) do {PUSH(pc.w); pc.w = n;} while(0)

#define COND_CALL(cond,n) do {if (cond) {CALL(n);/*dt += 3;*/};} while(0)

#define RST(n) do {PUSH(pc.w); pc.w = n;} while(0)

#define POP(r) do {r = m.read_word(sp.w); sp.w += 2;} while(0)

#define PUSH(r) do {sp.w -= 2;m.write_word(sp.w,r);} while(0)


#endif
