#include <GLFW/glfw3.h>

#include <GJGO/app.hpp>

namespace GJGO
{
    namespace Window
    {
        unsigned int getWidth()
        {
            int width;
            int height;
            glfwGetWindowSize(g_appInstancePtr->windowPtr, &width, &height);

            return static_cast<unsigned int>(width);
        }

        unsigned int getHeight()
        {
            int width;
            int height;
            glfwGetWindowSize(g_appInstancePtr->windowPtr, &width, &height);

            return height;
        }
    }
}
