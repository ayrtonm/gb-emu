#include <iostream>
#include <time.h> //clock_nanosleep, clock_gettime
#include "cpu.h"

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
}

//one cpu click is approximately 0.953674 microseconds
int cpu::emulate(mem &m, keypad &k, lcd &l, sound &s) {
  //local variables
  uint8 op;
  int dt = 0;
  tp.start_timer();
  for(;;)
  {
    if ((INT_VBL|INT_LCD|INT_TIM|INT_SER|INT_JOY) & m.read_byte_internal(O_IO+IO_IR) & m.read_byte_internal(O_IE)) {
      halt = false;
    }
    if (ime) {
      //find first set bit of interrupt request byte IO_IR
      for (int i = 1; i <= 0x20; i <<= 1) {
        //if bit i is both requested and enabled break out of the for loop to execute that interrupt
        //lowest set bit of IO_IR and IE has priority
        if ((m.read_byte_internal(O_IO + IO_IR) & i) && (m.read_byte_internal(O_IE) & i)) {
          //let's disable further interrupts
          ime = 0;
          ei_delay = 0;
          //clear the interrupt flag that was just triggered
          m.write_byte_internal(O_IO + IO_IR, m.read_byte_internal(O_IO + IO_IR) & ~i);
          //push the program counter onto the stack
          PUSH(pc.b.h,pc.b.l);
          pc.w = 0x40 + interrupt_table[i-1];
        }
      }
    }
    if (ei_delay) {ime = 1; ei_delay = 0;}
    if (halt) {dt = 1;}
    if (!halt) {
      op = m.read_byte(pc.w);
      if (op == 0xcb) {
        pc.w++;
        op = m.read_byte(pc.w++);
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
    switch (k.handle_events(dt, m)) {
      case none: {
        break;
      }
      case resize: {
        l.resize();
        break;
      }
      case quit: {
        if (verify_quit()) {
          if (state.saved) {
            delete state.m;
            delete state.l;
            delete state.k;
            delete state.s;
          }
          return 0;
        }
        break;
      }
      case boost: {
        tp.toggle_speed();
        break;
      }
      case savestate: {
        if (!state.saved) {
          state.m = new mem(m);
          state.l = new lcd(l);
          state.k = new keypad(k);
          state.s = new sound(s);
        }
        else {
          *state.m = m;
          *state.l = l;
          *state.k = k;
          *state.s = s;
        }
        state.saved = true;
        state.c.af = af;
        state.c.bc = bc;
        state.c.de = de;
        state.c.hl = hl;
        state.c.sp = sp;
        state.c.pc = pc;
        state.c.ime = ime;
        state.c.ei_delay = ei_delay;
        state.c.halt = halt;
        break;
      }
      case loadstate: {
        if (state.saved) {
          m = *state.m;
          l = *state.l;
          k = *state.k;
          s = *state.s;
          af = state.c.af;
          bc = state.c.bc;
          de = state.c.de;
          hl = state.c.hl;
          sp = state.c.sp;
          pc = state.c.pc;
          ime = state.c.ime;
          ei_delay = state.c.ei_delay;
          halt = state.c.halt;
        }
        break;
      }
    }
    tp.throttle(dt);
  }
  return 1;
}

bool cpu::verify_quit(void) {
  cout << "Are you sure you want to close gb-emu? [y/N]\n";
  SDL_Event event;
  do {
    SDL_WaitEvent(&event);
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
        case SDLK_y: {
          return true;
        }
        case SDLK_n: {
          return false;
        }
        default: {
          return false;
        }
      }
    }
  } while (event.type != SDL_KEYDOWN);
  //this is never reached, it's only used to prevent a compiler warning
  return false;
}

void throttle_controller::throttle(int dt) {
  throttleclk += dt;
  if (throttleclk >= throttlethreshold) {
    end_timer();
    throttleclk -= throttlethreshold;
    double elapsed = (tend.tv_sec - tstart.tv_sec) * 1000000.0;
    elapsed += (tend.tv_nsec - tstart.tv_nsec) / 1000.0;
    wait.tv_nsec = ((fudge_factor*throttlethreshold*cpuclk) - elapsed) * 1000.0;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &wait, NULL);
    start_timer();
  }
}

void throttle_controller::start_timer(void) {
  clock_gettime(CLOCK_MONOTONIC, &tstart);
}

void throttle_controller::end_timer(void) {
  clock_gettime(CLOCK_MONOTONIC, &tend);
}

void throttle_controller::toggle_speed(void) {
  if (fudge_factor == 0.25) {
    fudge_factor = 0.75;
  }
  else {
    fudge_factor = 0.25;
  }
}
