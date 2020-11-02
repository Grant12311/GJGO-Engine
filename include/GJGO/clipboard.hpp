#ifndef GJGO_CLIPBOARD_H
#define GJGO_CLIPBOARD_H

#include <string>

namespace GJGO
{
    namespace Clipboard
    {
        std::string readText();
        void writeText(const char* const a_text);
    }
}

#endif
