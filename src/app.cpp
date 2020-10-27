#include <GJGO/app.hpp>
#include <GJGO/log.hpp>

namespace GJGO
{
    void Application::run()
    {
        while (this->window.isOpen)
        {
            this->window.update();

            for (Layer* const l_layerPtr : this->layers)
            {
                l_layerPtr->onUpdate();
            }

            for (Event* const l_eventPtr : this->pendingEvents)
            {

            }

            this->window.clear();
            this->window.swapBuffers();
        }
    }

    Application::Application(const Hangar::Config &a_config) :
        window(a_config)
    {
        GJGO_LOG_INFO(glGetString(GL_VERSION));
    }
}
