#ifndef THROTTLE_H
#define THROTTLE_H

class throttle_controller {
  public:
    void throttle(int dt);
    void start_timer(void);
    void end_timer(void);
    void toggle_speed(void);
  private:
    double cpuclk = 0.953674;
    int throttleclk = 0;
    int throttlethreshold = 500;
    double fudge_factor = 0.75;
    struct timespec wait = {0, 0};
    struct timespec tstart, tend;
};

#endif
