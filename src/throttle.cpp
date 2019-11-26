#include <time.h> //clock_nanosleep, clock_gettime
#include "throttle.h"

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

void throttle_controller::toggle_speed(double fast, double slow) {
  fudge_factor = (fudge_factor == slow ? fast : slow);
}
