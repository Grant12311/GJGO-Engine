#include <GJGOpch.hpp>

#include <GJGO/app.hpp>
#include <GJGO/profiler.hpp>

namespace GJGO
{
    namespace Window
    {
        void setTitle(const char* const a_title)
        {
            GJGO_PROFILE_FUNCTION();

            glfwSetWindowTitle(Application::instance->windowPtr, a_title);
        }

        unsigned int getWidth()
        {
            GJGO_PROFILE_FUNCTION();

            int width;
            int height;
            glfwGetWindowSize(Application::instance->windowPtr, &width, &height);

            return static_cast<unsigned int>(width);
        }

        unsigned int getHeight()
        {
            GJGO_PROFILE_FUNCTION();

            int width;
            int height;
            glfwGetWindowSize(Application::instance->windowPtr, &width, &height);

            return height;
        }

        void setSize(const int a_width, const int a_height)
        {
            GJGO_PROFILE_FUNCTION();

            glfwSetWindowSize(Application::instance->windowPtr, a_width, a_height);
        }

        void maximize()
        {
            GJGO_PROFILE_FUNCTION();

            glfwMaximizeWindow(Application::instance->windowPtr);
        }

        void restore()
        {
            GJGO_PROFILE_FUNCTION();

            glfwRestoreWindow(Application::instance->windowPtr);
        }
    }
}
