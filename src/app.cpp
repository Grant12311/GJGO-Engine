#include "../include/GJGO/app.hpp"

namespace GJGO
{
    void Application::run()
    {
        while (this->window.isOpen)
        {
            this->window.update();
            //this->window.clear();
            this->window.swapBuffers();
        }
    }

    Application::Application()
    {

    }
}
