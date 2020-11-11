#ifndef GJGO_PROFILER_H
#define GJGO_PROFILER_H

#include <fstream>
#include <cstdint>
#include <string>
#include <algorithm>
#include <chrono>

#ifndef GJGO_BUILD_TARGET_DIST
    #define GJGO_PROFILE_SCOPE(x) GJGO::ProfilerTimer timer(x)
    #define GJGO_PROFILE_FUNCTION() GJGO::ProfilerTimer timer(__PRETTY_FUNCTION__)
#else
    #define GJGO_PROFILE_SCOPE(x)
    #define GJGO_PROFILE_FUNCTION()
#endif // GJGO_BUILD_TARGET_DIST

namespace GJGO
{
    struct ProfileResult
    {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct ProfilerSession
    {
        std::string Name;
    };

    class Profiler
    {
    private:
        ProfilerSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    public:
        Profiler()
            : m_CurrentSession(nullptr), m_ProfileCount(0) {}

        void BeginSession(const std::string& a_name, const std::string& a_filepath = "results.json")
        {
            this->m_OutputStream.open(a_filepath);
            this->WriteHeader();
            this->m_CurrentSession = new ProfilerSession{a_name};
        }

        void EndSession()
        {
            this->WriteFooter();
            this->m_OutputStream.close();
            delete this->m_CurrentSession;
            this->m_CurrentSession = nullptr;
            this->m_ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& a_result)
        {
            if (this->m_ProfileCount++ > 0)
                this->m_OutputStream << ",";

            std::string name = a_result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            this->m_OutputStream << "{";
            this->m_OutputStream << "\"cat\":\"function\",";
            this->m_OutputStream << "\"dur\":" << (a_result.End - a_result.Start) << ',';
            this->m_OutputStream << "\"name\":\"" << name << "\",";
            this->m_OutputStream << "\"ph\":\"X\",";
            this->m_OutputStream << "\"pid\":0,";
            this->m_OutputStream << "\"tid\":" << a_result.ThreadID << ",";
            this->m_OutputStream << "\"ts\":" << a_result.Start;
            this->m_OutputStream << "}";

            this->m_OutputStream.flush();
        }

        void WriteHeader()
        {
            this->m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            this->m_OutputStream.flush();
        }

        void WriteFooter()
        {
            this->m_OutputStream << "]}";
            this->m_OutputStream.flush();
        }

        static Profiler& Get()
        {
            static Profiler instance;
            return instance;
        }
    };

    class ProfilerTimer
    {
    public:
        void stop();

        ProfilerTimer(const char* name);
        ~ProfilerTimer();
    private:
        const char* m_name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
        bool m_stopped;
    };
}

#endif
