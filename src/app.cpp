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
                for (auto it = this->layers.rbegin(); it != this->layers.rend(); it++)
                {
                    (*it)->onEvent(l_eventPtr);
                }
            }

            this->window.clear();

            for (Layer* const l_layerPtr : this->layers)
            {
                l_layerPtr->draw();
            }

            this->window.swapBuffers();
        }
    }

    Application::Application(const Hangar::Config &a_config) :
        window(a_config)
    {
        GJGO_LOG_INFO(glGetString(GL_VERSION));
    }
}
