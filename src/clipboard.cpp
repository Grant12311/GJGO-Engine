#include <GJGOpch.hpp>

#include <GJGO/app.hpp>
#include <GJGO/profiler.hpp>

namespace GJGO
{
    namespace Clipboard
    {
        static std::string s_lastRead;

        const char* readText()
        {
            GJGO_PROFILE_FUNCTION();

            s_lastRead = glfwGetClipboardString(App::instance->windowPtr);
            return s_lastRead.c_str();
        }

        void writeText(const char* const a_text)
        {
            GJGO_PROFILE_FUNCTION();

            glfwSetClipboardString(App::instance->windowPtr, a_text);
        }
    }
}
