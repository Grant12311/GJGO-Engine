#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GJGO
{
    namespace Display
    {
        int getWidth()
        {
            return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
        }

        int getHeight()
        {
            glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
        }
    }
}
