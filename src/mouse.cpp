#include <GJGOpch.hpp>

#include <GJGO/2D/transform2D.hpp>
#include <GJGO/app.hpp>
#include <GJGO/display.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    namespace Mouse
    {
        Position2D getPositionRelative()
        {
            GJGO_PROFILE_FUNCTION();

            double posX, posY;
            glfwGetCursorPos(App::instance->windowPtr, &posX, &posY);

            return {static_cast<int>(posX), static_cast<int>(GJGO::Window::getHeight()) - static_cast<int>(posY) - 1};
        }

        Position2D getPositionAbsolute()
        {
            GJGO_PROFILE_FUNCTION();

            double posX, posY;
            glfwGetCursorPos(App::instance->windowPtr, &posX, &posY);

            Position2D windowPosition = Window::getPosition();

            return {windowPosition.x + static_cast<int>(posX), static_cast<int>(GJGO::Window::getHeight()) - static_cast<int>(posY) - 1 + windowPosition.y};
        }
    }
}
