#include <iostream>
#include <unistd.h> //usleep
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

cpu::cpu() {
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
  cputhrottleclk = 0;
}

//one cpu click is approximately 0.953674 microseconds
int cpu::emulate(mem &m) {
  //local variables
  int op;
  int dt = 0;

  //initialize subsystem classes
  lcd *l;
  l = new lcd;
  keypad *k;
  k = new keypad;
  snd *s;
  s = new snd;

  for(;;)
  {
#ifdef PRINT_REGISTERS
    cin.get();
    print_registers(m);
#endif
    if ((INT_VBL|INT_LCD|INT_TIM|INT_SER|INT_JOY) & m.read_byte(O_IO+IO_IR) & m.read_byte(O_IE)) {
      halt = false;
    }
    if (ime) {
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
        }
      }
    }
    if (ei_delay) {ime = 1; ei_delay = 0;}
    if (halt) {dt = 1;}
    if (!halt) {
      op = (int)m.read_byte(pc.w);
#ifdef DEBUG
      cout << "[0x" << hex << (int) pc.w << "]  " << "0x" << hex << (int) op; 
      for (int i = length[op]; i > 1; i--) {
        cout << " 0x" << hex << (int) m.read_byte(pc.w+length[op]-i+1);
      }
      cout << endl;
      cout << "stack is pointing at 0x" << hex <<(int)sp.w << endl;
#endif
      if (op == 0xcb) {
        pc.w++;
        op = (int)m.read_byte(pc.w++);
        dt = cb_cycles[op & 0x07];
        switch(op) {
          #include "cb_opcodes.h"
        }
      }
      else {
        pc.w += length[op];
        dt = cycles[op];
        switch(op) {
          #include "opcodes.h"
        }
      }
    }
    if (repeat) {repeat = false; pc.w -= length[op]-1;}
    m.update_timers(dt);
    l->step_lcd(dt,m);
    switch (k->handle_events(m)) {
      case none: {
        break;
      }
      case resize: {
        l->resize();
        break;
      }
      case quit: {
        delete l;
        delete k;
        delete s;
        return 0;
      }
    }
    throttle(dt);
  }
  delete l;
  delete k;
  return 1;
}

void cpu::print_registers(mem &m) {
  cout << "AF: 0x" << hex <<  (int) af.w << endl;
  cout << "BC: 0x" << hex <<  (int) bc.w << endl;
  cout << "DE 0x" << hex <<  (int) de.w << endl;
  cout << "HL 0x" << hex <<  (int) hl.w << endl;
  cout << "SP 0x" << hex <<  (int) sp.w << endl;
  cout << "PC 0x" << hex <<  (int) pc.w << endl;
  cout << "[PC] 0x" << hex << (int) m.read_byte(pc.w);
  for (int i = length[m.read_byte(pc.w)]; i > 1; i--) {
    cout << " 0x" << hex << (int) m.read_byte(pc.w+length[m.read_byte(pc.w)]-i+1);
  }
  cout << endl;
  cout << "Zero flag is " << (af.b.l & F_Z ? "set" : "clear") << endl;
  cout << "N flag is " << (af.b.l & F_N ? "set" : "clear") << endl;
  cout << "H flag is " << (af.b.l & F_H ? "set" : "clear") << endl;
  cout << "Carry flag is " << (af.b.l & F_C ? "set" : "clear") << endl;
  return;
}

void cpu::throttle(int dt) {
  cputhrottleclk += dt;
  if(cputhrottleclk >= CPU_CLKS) {
    cputhrottleclk -= CPU_CLKS;
    usleep(1000);
  }
}
