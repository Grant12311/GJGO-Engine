#include <string>

#include <clip.h>

namespace GJGO
{
    namespace Clipboard
    {
        static std::string s_lastRead;

        const char* readText()
        {
            clip::get_text(s_lastRead);
            return s_lastRead.c_str();
        }

        void writeText(const char* const a_text)
        {
            clip::set_text(a_text);
        }
    }
}
