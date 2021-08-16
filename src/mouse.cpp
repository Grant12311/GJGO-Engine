#include <GJGO/app.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    namespace Mouse
    {
        [[nodiscard]]
        glm::vec2 getPosition()
        {
            double posX, posY;
            glfwGetCursorPos(App::instance->window, &posX, &posY);
            return {posX, Window::getHeight() - posY};
        }

        [[nodiscard]]
        float getPositionX()
        {
            double posX, posY;
            glfwGetCursorPos(App::instance->window, &posX, &posY);
            return posX;
        }

        [[nodiscard]]
        float getPositionY()
        {
            double posX, posY;
            glfwGetCursorPos(App::instance->window, &posX, &posY);
            return posY;
        }
    }
}
