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

            glfwSetWindowTitle(App::instance->windowPtr, a_title);
        }

        std::array<int, 2> getPosition()
        {
            std::array<int, 2> toReturn;
            glfwGetWindowPos(App::instance->windowPtr, &toReturn[0], &toReturn[1]);
            return toReturn;
        }

        unsigned int getWidth()
        {
            GJGO_PROFILE_FUNCTION();

            int width;
            int height;
            glfwGetWindowSize(App::instance->windowPtr, &width, &height);

            return static_cast<unsigned int>(width);
        }

        unsigned int getHeight()
        {
            GJGO_PROFILE_FUNCTION();

            int width;
            int height;
            glfwGetWindowSize(App::instance->windowPtr, &width, &height);

            return height;
        }

        void setSize(const int a_width, const int a_height)
        {
            GJGO_PROFILE_FUNCTION();

            glfwSetWindowSize(App::instance->windowPtr, a_width, a_height);
        }

        void maximize()
        {
            GJGO_PROFILE_FUNCTION();

            glfwMaximizeWindow(App::instance->windowPtr);
        }

        void restore()
        {
            GJGO_PROFILE_FUNCTION();

            glfwRestoreWindow(App::instance->windowPtr);
        }
    }
}
