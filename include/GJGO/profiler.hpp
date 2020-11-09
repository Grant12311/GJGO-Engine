#ifndef GJGO_PROFILER_H
#define GJGO_PROFILER_H

#include <chrono>

namespace GJGO
{
    class ProfilerTimer
    {
    private:
        const char* const m_name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
        bool m_stopped;
    public:
        void stop();

        ProfilerTimer(const char* const a_name);
        ~ProfilerTimer();
    };
}

#endif
