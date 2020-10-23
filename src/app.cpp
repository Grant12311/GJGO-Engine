#include <GJGO/app.hpp>
#include <GJGO/log.hpp>

namespace GJGO
{
    void Application::run()
    {
        GJGO_LOG_INFO(glGetString(GL_VERSION));

        while (this->window.isOpen)
        {
            this->window.update();
            this->onUpdateEvent.call(this);
            this->window.clear();
            this->window.swapBuffers();
        }
    }

    Application::Application(const Hangar::Config &a_config) :
        window(a_config) {}
}
