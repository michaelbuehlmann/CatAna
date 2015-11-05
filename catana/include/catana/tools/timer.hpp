#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include "timer.hpp"

class Timer
{
public:
    Timer();
    void start(); // Sets t_start
    void stop(); // Sets t_stop
    double duration_s(); // Returns duration in seconds; start and stop must have been executed
    double duration_ms();
    double duration_us();
    double duration_ns();
private:
    std::chrono::high_resolution_clock::time_point t_start;
    std::chrono::high_resolution_clock::time_point t_stop;
    bool started;
    bool stopped;
};

#endif