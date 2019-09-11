#include "generic.h"
#include "states.h"

void generic_cpu::save_state(emulator_state *st, mem &m, keypad &k, lcd &l, sound &s) {
  if (!st->saved) {
    st->m = new mem(m);
    st->l = new lcd(l);
    st->k = new keypad(k);
    st->s = new sound(s);
  }
  else {
    *st->m = m;
    *st->l = l;
    *st->k = k;
    *st->s = s;
  }
  st->saved = true;
  st->c.af = af;
  st->c.bc = bc;
  st->c.de = de;
  st->c.hl = hl;
  st->c.sp = sp;
  st->c.pc = pc;
  st->c.ime = ime;
  st->c.ei_delay = ei_delay;
  st->c.halt = halt;
}

void generic_cpu::load_state(emulator_state *st, mem &m, keypad &k, lcd &l, sound &s) {
  if (st->saved) {
    m = *st->m;
    l = *st->l;
    l.resize();
    k = *st->k;
    s = *st->s;
    af = st->c.af;
    bc = st->c.bc;
    de = st->c.de;
    hl = st->c.hl;
    sp = st->c.sp;
    pc = st->c.pc;
    ime = st->c.ime;
    ei_delay = st->c.ei_delay;
    halt = st->c.halt;
  }
}

void generic_cpu::delete_state(emulator_state *st) {
  if (st->saved) {
    delete st->m;
    delete st->l;
    delete st->k;
    delete st->s;
  }
  delete st;
}

