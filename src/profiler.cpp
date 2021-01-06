#include <GJGOpch.hpp>

#include <GJGO/profiler.hpp>

namespace GJGO
{
    void ProfilerTimer::stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(this->m_startTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Profiler::Get().WriteProfile({this->m_name, start, end, threadID});

        this->m_stopped = true;
    }

    ProfilerTimer::ProfilerTimer(const char* name)
        : m_name(name), m_startTimepoint(std::chrono::high_resolution_clock::now()), m_stopped(false) {}

    ProfilerTimer::~ProfilerTimer()
    {
        if (!this->m_stopped)
            this->stop();
    }
}
