#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GJGO
{
    namespace Display
    {
        [[nodiscard]]
        int getWidth()
        {
            return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
        }

        [[nodiscard]]
        int getHeight()
        {
            return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
        }
    }
}
