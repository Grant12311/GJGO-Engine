#include <GJGOpch.hpp>

#include <GJGO/2D/transform2D.hpp>
#include <GJGO/app.hpp>
#include <GJGO/display.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    namespace Window
    {
        void setTitle(const char* const a_title)
        {
            GJGO_PROFILE_FUNCTION();

            glfwSetWindowTitle(App::instance->windowPtr, a_title);
        }

        Position2D getPosition()
        {
            GJGO_PROFILE_FUNCTION();

            Position2D toReturn;
            glfwGetWindowPos(App::instance->windowPtr, &toReturn.x, &toReturn.y);
            toReturn.y = Display::getHeight() - Window::getHeight() - toReturn.y;

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

        Size2D getSize()
        {
           GJGO_PROFILE_FUNCTION();

            int width;
            int height;
            glfwGetWindowSize(App::instance->windowPtr, &width, &height);

            return {static_cast<unsigned int>(width), static_cast<unsigned int>(height)};
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
