#include <GLFW/glfw3.h>

namespace GJGO
{
    namespace Display
    {
        unsigned int getWidth()
        {
            return static_cast<unsigned int>(glfwGetVideoMode(glfwGetPrimaryMonitor())->width);
        }

        unsigned int getHeight()
        {
            return static_cast<unsigned int>(glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
        }
    }
}
