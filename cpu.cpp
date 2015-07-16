#include <iostream>
#include <cassert>
#include "cpu.h"

using namespace std;

cpu::cpu()
{
  af.b.l = 0x01;
  bc.w = 0x0013;
  de.w = 0x00d8;
  hl.w = 0x014d;
  sp.w = 0xfffe;
  pc.w = 0x0100;
  cout << "cpu initialized\n";
}

void cpu::print_registers(void)
{
  cout << "AF: 0x" << hex <<  (int) af.w << '\n';
  cout << "BC: 0x" << hex <<  (int) bc.w << '\n';
  cout << "DE 0x" << hex <<  (int) de.w << '\n';
  cout << "HL 0x" << hex <<  (int) hl.w << '\n';
  cout << "SP 0x" << hex <<  (int) sp.w << '\n';
  cout << "PC 0x" << hex <<  (int) pc.w << '\n';
  return;
}

int cpu::emulate(mem &m, lcd &l)
{
  m.format = l.screen->format;
  char next;
  uint8 op;
  uint8 dt = 0;
  for(;;)
  {
    if (halt || ime)
    {
      int i = 1;
      while ((!(m.io.at(_IR) & i) || !(m.io.at(_IR) & i)) && (i <= 0x10))
      {
        i <<= 1;
      }
      if (i <= 0x10)
      {
        if (halt) {pc.w++;}
        if (ime)
        {
          ime = 0;
          CLEAR(i,m.io.at(_IR));
          ei_delay = 0;
          PUSH(pc.b.h,pc.b.l);
          pc.w = 0x40 + interrupt_table[i-1];
        }
      }
    }
    if (halt) {dt = 4;}
    if (ei_delay) {ime = 1; ei_delay = 0;}
    if (!halt)
    {
      op = m.read_byte(pc.w);
#ifdef DEBUG
      cout << "[0x" << hex << (int) pc.w << "]" << "0x" << hex << (int) op; 
      for (int i = length[op]; i > 1; i--)
      {
        cout << " 0x" << hex << (int) m.read_byte(pc.w+length[op]-i+1);
      }
      cout << "\n";
#endif
      if (op == 0xcb)
      {
        pc.w++;
        op = m.read_byte(pc.w++);
        dt = cb_cycles[op & 0x07];
        switch(op)
        {
          #include "cb_opcodes.h"
        }
      }
      else
      {
        pc.w += length[op];
        dt = cycles[op];
        switch(op)
        {
          #include "opcodes.h"
        }
      }
    }
    l.step_lcd(dt,m);
    assert(l.parse_events());
  }
  return 1;
}