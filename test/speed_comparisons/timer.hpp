#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include "timer.hpp"

namespace timer {

    class Timer {
    public:
        Timer()
                :started(false), stopped(false) { }

        void start()
        {
            t_start = std::chrono::high_resolution_clock::now();
            started = true;
        }

        void stop()
        {
            t_stop = std::chrono::high_resolution_clock::now();
            stopped = true;
        }

        double duration()
        {
            double time;
            if (started && stopped) {
                std::chrono::duration<double> elapsed_seconds = t_stop-t_start;
                time = elapsed_seconds.count();
            } else {
                time = 0;
            }
            started = stopped = false;
            return time;
        }

    private:
        std::chrono::high_resolution_clock::time_point t_start;
        std::chrono::high_resolution_clock::time_point t_stop;
        bool started;
        bool stopped;
    };

}

#endif