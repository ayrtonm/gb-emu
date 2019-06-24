#include <iostream>
#include <unistd.h> //usleep
#include <time.h>
#include "cpu.h"
//ideally the CPU should run at 4 MHz meaning CPU_CLKS/CPU_SLEEP = 4
//realistically the ratio should be such that each call to usleep isn't noticeable (CPU_SLEEP isn't too long)
//and real cpu usage is low
//the screenupdateclk threshold can then be set based on CPU_SLEEP time so that the screen refreshes at approximately 60 Hz
//I should add a function to automatically set this ratio to minimize cpu usage
#define CPU_CLKS 5000
#define NS_PER_CLK 953.674

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
  lcd *l = new lcd;
  keypad *k = new keypad;
  sound *s = new sound;

  wait.tv_sec = 0;
  sleep_factor[0] = 0.6;
  sleep_factor[1] = 0.01;
  sleep_factor[2] = 50;
  sleep_factor[3] = 0.1;
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
    l->step_lcd(dt,m);
    switch (k->handle_events(m,sleep_factor)) {
      case none: {
        break;
      }
      case resize: {
        l->resize();
        break;
      }
      case quit: {
        cout << sleep_factor[0];
        delete l;
        delete k;
        delete s;
        return 0;
      }
    }
    throttle(dt);
  }
  return 1;
}

void cpu::throttle(int dt) {
  cputhrottleclk += dt;
  if(cputhrottleclk >= CPU_CLKS) {
    cputhrottleclk -= CPU_CLKS;
    wait.tv_nsec = CPU_CLKS*NS_PER_CLK*sleep_factor[0];
    clock_nanosleep(CLOCK_MONOTONIC, 0, &wait, NULL);
  }
}
