#include <string>

#include <clip.h>

namespace GJGO
{
    namespace Clipboard
    {
        std::string readText()
        {
            std::string toReturn;
            clip::get_text(toReturn);
            return toReturn;
        }

        void writeText(const char* const a_text)
        {
            clip::set_text(a_text);
        }
    }
}
