#include <pch.hpp>

#include <GJGO/profiler.hpp>

namespace GJGO
{
    namespace Display
    {
        unsigned int getWidth()
        {
            GJGO_PROFILE_FUNCTION();

            return static_cast<unsigned int>(glfwGetVideoMode(glfwGetPrimaryMonitor())->width);
        }

        unsigned int getHeight()
        {
            GJGO_PROFILE_FUNCTION();

            return static_cast<unsigned int>(glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
        }
    }
}
