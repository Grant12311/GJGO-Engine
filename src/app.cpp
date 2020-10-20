#include "../include/GJGO/app.hpp"

namespace GJGO
{
    void Application::run()
    {
        #ifdef GJGO_BUILD_TARGET_DEBUG
            std::cout << glGetString(GL_VERSION) << std::endl;
        #endif // GJGO_BUILD_TARGET_DEBUG

        while (this->window.isOpen)
        {
            this->window.update();
            this->window.clear();
            this->window.swapBuffers();
        }
    }

    Application::Application(const Hangar::Config &a_config) :
        window(a_config)
    {

    }
}
