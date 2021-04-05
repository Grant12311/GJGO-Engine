#include <GJGO/app.hpp>

namespace GJGO
{
    namespace Keyboard
    {
        bool keyIsDown(const int a_key)
        {
            return glfwGetKey(App::instance->window, a_key);
        }
    }
}
