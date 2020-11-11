#include <string>

#include <clip.h>

#include <GJGO/profiler.hpp>

namespace GJGO
{
    namespace Clipboard
    {
        static std::string s_lastRead;

        const char* readText()
        {
            GJGO_PROFILE_FUNCTION();

            clip::get_text(s_lastRead);
            return s_lastRead.c_str();
        }

        void writeText(const char* const a_text)
        {
            GJGO_PROFILE_FUNCTION();

            clip::set_text(a_text);
        }
    }
}
