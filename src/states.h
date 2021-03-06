#ifndef STATES_H
#define STATES_H
#include "bits.h"
#include "mem.h"
#include "lcd.h"
#include "keypad.h"
#include "sound.h"

typedef struct cpu_state {
  word16 af, bc, de, hl, sp, pc;
  uint8_t ime;
  uint8_t ei_delay;
  uint8_t halt;
  bool repeat;
} cpu_state;

typedef struct emulator_state {
  cpu_state c;
  mem *m;
  lcd *l;
  keypad *k;
  sound *s;
  bool saved = false;
} emulator_state;


#endif
