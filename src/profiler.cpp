#include <iostream>

#include <GJGO/profiler.hpp>

namespace GJGO
{
    void ProfilerTimer::stop()
    {
        auto endPoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(this->m_startPoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

        this->m_stopped = true;
        std::cout << "Duration: " << (end - start) * 0.001f << "ms" <<std::endl;
    }

    ProfilerTimer::ProfilerTimer(const char* const a_name) :
        m_name(a_name), m_startPoint(std::chrono::high_resolution_clock::now()), m_stopped(false) {}

    ProfilerTimer::~ProfilerTimer()
    {
        if (!this->m_stopped)
            this->stop();
    }
}
