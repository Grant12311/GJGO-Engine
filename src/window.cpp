#include <pch.hpp>

#include <GJGO/app.hpp>
#include <GJGO/profiler.hpp>

namespace GJGO
{
    namespace Window
    {
        void setTitle(const char* const a_title)
        {
            GJGO_PROFILE_FUNCTION();

            glfwSetWindowTitle(g_appInstancePtr->windowPtr, a_title);
        }

        unsigned int getWidth()
        {
            GJGO_PROFILE_FUNCTION();

            int width;
            int height;
            glfwGetWindowSize(g_appInstancePtr->windowPtr, &width, &height);

            return static_cast<unsigned int>(width);
        }

        unsigned int getHeight()
        {
            GJGO_PROFILE_FUNCTION();

            int width;
            int height;
            glfwGetWindowSize(g_appInstancePtr->windowPtr, &width, &height);

            return height;
        }

        void setSize(const int a_width, const int a_height)
        {
            GJGO_PROFILE_FUNCTION();

            glfwSetWindowSize(g_appInstancePtr->windowPtr, a_width, a_height);
        }
    }
}
