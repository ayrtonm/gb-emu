#include <iostream>
#include <time.h> //clock_nanosleep, clock_gettime
#include "cpu.h"
#define RATE 500

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
int cpu::emulate(mem &m, keypad &k, lcd &l, sound &s) {
  waitlong.tv_nsec = 1000000;
  waitlong.tv_sec = 0;
  waitshort.tv_nsec = 10000;
  waitshort.tv_sec = 0;
  waitvshort.tv_nsec =  10000;
  waitvshort.tv_sec = 0;
  //local variables
  int op;
  int dt = 0;

  clock_gettime(CLOCK_MONOTONIC, &tstart);
  for(;;)
  {
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
        }
      }
    }
    if (ei_delay) {ime = 1; ei_delay = 0;}
    if (halt) {dt = 1;}
    if (!halt) {
      op = (int)m.read_byte(pc.w);
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
    m.update_timers(dt*4);
    l.step_lcd(dt,m);
    switch (k.handle_events(m)) {
      case none: {
        break;
      }
      case resize: {
        l.resize();
        break;
      }
      case quit: {
        return 0;
      }
    }
    throttle(dt);
  }
  return 1;
}

void cpu::throttle(int dt) {
  cputhrottleclk += dt;
  if(cputhrottleclk >= RATE) {
    cputhrottleclk -= RATE;
    clock_gettime(CLOCK_MONOTONIC, &tend);
    double elapsed = (tend.tv_sec - tstart.tv_sec) * 1000000.0;
    elapsed += (tend.tv_nsec - tstart.tv_nsec) / 1000.0;
    //elapsed /= dt;
    //cout << elapsed << endl;
    if (elapsed < 1.1*RATE) {
      clock_nanosleep(CLOCK_MONOTONIC, 0, &waitlong, NULL);
    }
    else if (elapsed < 1.7*RATE) {
      clock_nanosleep(CLOCK_MONOTONIC, 0, &waitshort, NULL);
    }
    else {
      clock_nanosleep(CLOCK_MONOTONIC, 0, &waitvshort, NULL);
    }
    tstart = tend;
  }
}
