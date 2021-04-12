#include <GJGO/app.hpp>
#include <GJGO/display.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    namespace Window
    {
        void setTitle(const char* const a_title)
        {
            glfwSetWindowTitle(App::instance->window, a_title);
        }

        float getPositionX()
        {
            return getPosition().x;
        }

        float getPositionY()
        {
            return getPosition().y;
        }

        glm::vec2 getPosition()
        {
            int x, y;
            glfwGetWindowPos(App::instance->window, &x, &y);
            y = Display::getHeight() - Window::getHeight() - y;

            return {x, y};
        }

        float getWidth()
        {
            return getSize().x;
        }

        float getHeight()
        {
            return getSize().y;
        }

        glm::vec2 getSize()
        {
            int width;
            int height;
            glfwGetWindowSize(App::instance->window, &width, &height);

            return {width, height};
        }

        void setSize(const int a_width, const int a_height)
        {
            glfwSetWindowSize(App::instance->window, a_width, a_height);
        }

        void maximize()
        {
            glfwMaximizeWindow(App::instance->window);
        }

        void restore()
        {
            glfwRestoreWindow(App::instance->window);
        }
    }
}
