#include "generic_cpu.h"
#include "states.h"

using namespace std;

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

bool generic_cpu::matches_state(cpu_state *st) {
  bool same = true;
  uint16_t af_diff = af.w - st->af.w;
  //let's ignore the H flag for now since the dynarec doesn't handle it yet
  af_diff &= ~F_H;
  if (af_diff != 0) {
    cout << "cpu state differs in AF register " << hex << af.w << " != " << hex << st->af.w << endl;
    same = false;
  }
  uint16_t bc_diff = bc.w - st->bc.w;
  if (bc_diff != 0) {
    cout << "cpu state differs in BC register " << hex << bc.w << " != " << hex << st->bc.w << endl;
    same = false;
  }
  uint16_t de_diff = de.w - st->de.w;
  if (de_diff != 0) {
    cout << "cpu state differs in DE register " << hex << de.w << " != " << hex << st->de.w << endl;
    same = false;
  }
  uint16_t hl_diff = hl.w - st->hl.w;
  if (hl_diff != 0) {
    cout << "cpu state differs in HL register " << hex << hl.w << " != " << hex << st->hl.w << endl;
    same = false;
  }
  uint16_t sp_diff = sp.w - st->sp.w;
  if (sp_diff != 0) {
    cout << "cpu state differs in SP register " << hex << sp.w << " != " << hex << st->sp.w << endl;
    same = false;
  }
  uint16_t pc_diff = pc.w - st->pc.w;
  if (pc_diff != 0) {
    cout << "cpu state differs in PC register " << hex << pc.w << " != " << hex << st->pc.w << endl;
    same = false;
  }
  return same;
}

cpu_state *generic_cpu::dump_cpu_state() {
  cpu_state *st = new cpu_state;
  st->af = af;
  st->bc = bc;
  st->de = de;
  st->hl = hl;
  st->sp = sp;
  st->pc = pc;
  return st;
}
