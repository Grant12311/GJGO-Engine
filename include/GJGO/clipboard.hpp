#ifndef GJGO_CLIPBOARD_H
#define GJGO_CLIPBOARD_H

#include <string>

namespace GJGO
{
    namespace Clipboard
    {
        const char* readText();
        void writeText(const char* const a_text);
    }
}

#endif
