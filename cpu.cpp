#include <iostream>
#include <cassert>
#include <time.h>
#include <unistd.h>
#include "cpu.h"
//ideally the CPU should run at 4 MHz meaning CPU_CLKS/CPU_SLEEP = 4
//realistically the ratio should be such that each call to usleep isn't noticeable (CPU_SLEEP isn't too long)
//and real cpu usage is low
//the screenupdateclk threshold can then be set based on CPU_SLEEP time so that the screen refreshes at approximately 60 Hz
//I should add a function to automatically set this ratio to minimize cpu usage
#define CPU_CLKS 1000
//in nanoseconds
//#define CPU_SLEEP 2
//#define DEBUG
//#define PRINT_REGISTERS
//#define PRINT_INTERRUPTS

using namespace std;

cpu::cpu()
{
  //af.b.l = 0x01;
  af.w = 0x01b0;
  bc.w = 0x0013;
  de.w = 0x00d8;
  hl.w = 0x014d;
  sp.w = 0xfffe;
  pc.w = 0x0100;
  halt = 0;
  ime = 0;
  ei_delay = 0;
  repeat = false;
  cout << "cpu initialized\n";
}

void cpu::print_registers(mem &m)
{
  cout << "AF: 0x" << hex <<  (int) af.w << endl;
  cout << "BC: 0x" << hex <<  (int) bc.w << endl;
  cout << "DE 0x" << hex <<  (int) de.w << endl;
  cout << "HL 0x" << hex <<  (int) hl.w << endl;
  cout << "SP 0x" << hex <<  (int) sp.w << endl;
  cout << "PC 0x" << hex <<  (int) pc.w << endl;
  cout << "[PC] 0x" << hex << (int) m.read_byte(pc.w);
  for (int i = length[m.read_byte(pc.w)]; i > 1; i--)
  {
    cout << " 0x" << hex << (int) m.read_byte(pc.w+length[m.read_byte(pc.w)]-i+1);
  }
  cout << endl;
  cout << "Zero flag is " << (af.b.l & F_Z ? "set" : "clear") << endl;
  cout << "N flag is " << (af.b.l & F_N ? "set" : "clear") << endl;
  cout << "H flag is " << (af.b.l & F_H ? "set" : "clear") << endl;
  cout << "Carry flag is " << (af.b.l & F_C ? "set" : "clear") << endl;
  return;
}

//one cpu click is approximately 0.953674 microseconds
int cpu::emulate(mem &m)
{
  int op;
  int dt = 0;
  int cputhrottleclk = 0;
  //struct timespec wait; 
  //wait.tv_sec = 0;

  lcd *l;
  l = new lcd;
  for(;;)
  {
#ifdef PRINT_REGISTERS
    cin.get();
    //system("clear");
    print_registers(m);
#endif
    if ((INT_VBL|INT_LCD|INT_TIM|INT_SER|INT_JOY) & m.read_byte(O_IO+IO_IR) & m.read_byte(O_IE)) {
      halt = false;
    }
    if (ime)
    {
      //find first set bit of interrupt request byte IO_IR
      for (int i = 1; i <= 0x20; i <<= 1) {
        //if bit i is both requested and enabled break out of the for loop to execute that interrupt
        //lowest set bit of IO_IR and IE has priority
        if ((m.read_byte(O_IO + IO_IR) & i) && (m.read_byte(O_IE) & i)) {
          //let's disable further interrupts
          ime = 0;
          ei_delay = 0;
          //clear the interrupt flag that was just triggered
          m.write_byte_internal(O_IO + IO_IR, m.read_byte(O_IO + IO_IR) & ~i);
          //push the program counter onto the stack
          PUSH(pc.b.h,pc.b.l);
          pc.w = 0x40 + interrupt_table[i-1];
#ifdef PRINT_INTERRUPTS
          cout << "executed interrupt 0x" << hex << (int)i << endl;
#endif
          //dt = 3;
          //if halted let's push program counter + 1 onto the stack since this is where we want to return
          //if (halt) {pc.w++;}
          //halt = 0;
          //if interrupts enabled
          //jump to location based on lookup table for that interrupt (avoids having to take log2)
        }
      }
    }
    if (ei_delay) {ime = 1; ei_delay = 0;}
    if (halt) {dt = 1;}
    if (!halt)
    {
      op = (int)m.read_byte(pc.w);
#ifdef DEBUG
      cout << "[0x" << hex << (int) pc.w << "]  " << "0x" << hex << (int) op; 
      for (int i = length[op]; i > 1; i--)
      {
        cout << " 0x" << hex << (int) m.read_byte(pc.w+length[op]-i+1);
      }
      cout << endl;
      cout << "stack is pointing at 0x" << hex <<(int)sp.w << endl;
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
    if (repeat) {repeat = false; pc.w -= length[op]-1;}
    m.update_timers(dt);
    l->step_lcd(dt,m);
    if(!l->parse_events(m)) {
      return 0;
      delete l;
    };
    cputhrottleclk += dt;
    if(cputhrottleclk >= CPU_CLKS) {
      //wait.tv_nsec = 1000;
      cputhrottleclk -= CPU_CLKS;
      //nanosleep(&wait, NULL);
      usleep(1000);
    }
  }
  delete l;
  return 1;
}
