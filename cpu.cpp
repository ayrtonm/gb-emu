#include <iostream>
#include <cassert>
#include <time.h>
#include "cpu.h"
//ideally the CPU should run at 4 MHz meaning CPU_CLKS/CPU_SLEEP = 4
//realistically the ratio should be such that each call to usleep isn't noticeable (CPU_SLEEP isn't too long)
//and real cpu usage is low
//the screenupdateclk threshold can then be set based on CPU_SLEEP time so that the screen refreshes at approximately 60 Hz
//I should add a function to automatically set this ratio to minimize cpu usage
#define CPU_CLKS 25
//in nanoseconds
//#define CPU_SLEEP 2
//#define DEBUG

using namespace std;

cpu::cpu()
{
  af.b.l = 0x01;
  bc.w = 0x0013;
  de.w = 0x00d8;
  hl.w = 0x014d;
  sp.w = 0xfffe;
  pc.w = 0x0100;
  halt = 0;
  ime = 1;
  ei_delay = 0;
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
  int op;
  int dt = 0;
  int cputhrottleclk = 0;
  struct timespec wait; 
  wait.tv_sec = 0;
  for(;;)
  {
    if (halt || ime)
    {
      //find first set bit of interrupt request byte IO_IR
      for (int i = 1; i <= 0x10; i <<= 1) {
        //if bit i is both requested and enabled break out of the for loop to execute that interrupt
        //lowest set bit of IO_IR and IE has priority
        if ((m.read_byte(O_IO + IO_IR) & i) && (m.read_byte(O_IE) & i)) {
          if (halt) {pc.w++;}
          if (ime)
          {
            ime = 0;
            m.write_byte(O_IO + IO_IR, m.read_byte(O_IO + IO_IR) & ~i);
            ei_delay = 0;
            PUSH(pc.b.h,pc.b.l);
            pc.w = 0x40 + interrupt_table[i-1];
          }
        }
      }
    }
    if (halt) {dt = 4;}
    if (ei_delay) {ime = 1; ei_delay = 0;}
    if (!halt)
    {
      op = (int)m.read_byte(pc.w);
#ifdef DEBUG
      cout << "[0x" << hex << (int) pc.w << "]  " << "0x" << hex << (int) op; 
      for (int i = length[op]; i > 1; i--)
      {
        cout << " 0x" << hex << (int) m.read_byte(pc.w+length[op]-i+1);
      }
      cout << "\n";
#endif
      if (op == 0xcb)
      {
        pc.w++;
        op = (int)m.read_byte(pc.w++);
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
    m.update_timers(dt);
    l.step_lcd(dt,m);
    if(!l.parse_events(m)) return 0;
    cputhrottleclk += dt;
    if(cputhrottleclk >= CPU_CLKS) {
      wait.tv_nsec = 1;
      cputhrottleclk -= CPU_CLKS;
      clock_nanosleep(CLOCK_MONOTONIC,0, &wait, NULL);
    }
  }
  return 1;
}
