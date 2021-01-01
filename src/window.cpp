#include <GLFW/glfw3.h>

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
    }
}
