#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include "timer.hpp"

namespace timer {
    using seconds = std::chrono::seconds;
    using milliseconds = std::chrono::milliseconds;
    using microseconds = std::chrono::microseconds;
    using nanoseconds = std::chrono::nanoseconds;

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

        template<class T>
        double duration()
        {
            if (started && stopped) {
                return std::chrono::duration_cast<T>(t_stop-t_start).count();
            } else {
                return 0;
            }
        }

    private:
        std::chrono::high_resolution_clock::time_point t_start;
        std::chrono::high_resolution_clock::time_point t_stop;
        bool started;
        bool stopped;
    };

}

#endif