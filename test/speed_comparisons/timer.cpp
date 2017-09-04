#include "timer.hpp"


Timer::Timer()
    : started(false), stopped(false) {}

void Timer::start() {
  t_start = std::chrono::high_resolution_clock::now();
  started = true;
}

void Timer::stop() {
  t_stop = std::chrono::high_resolution_clock::now();
  stopped = true;
}

double Timer::duration_s() {
  if(started && stopped) {
    return std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start).count() / 1000000.;
  } else {
    return 0;
  }
}

double Timer::duration_ms() {
  if(started && stopped) {
    return std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start).count() / 1000.;
  } else {
    return 0;
  }
}

double Timer::duration_us() {
  if(started && stopped) {
    return std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start).count();
  } else {
    return 0;
  }
}

double Timer::duration_ns() {
  if(started && stopped) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t_stop - t_start).count();
  } else {
    return 0;
  }
}