#include <GJGO/app.hpp>
#include <GJGO/event.hpp>
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

            for (unsigned int i = 0; i < this->pendingEvents.size(); i++)
            {
                Event* const currentEvent = this->pendingEvents[i];

                for (auto it = this->layers.rbegin(); it != this->layers.rend(); it++)
                {
                    (*it)->onEvent(currentEvent);
                    if (currentEvent->handled)
                        break;
                }

                delete currentEvent;
                this->pendingEvents.erase(this->pendingEvents.begin() + i);
                i--;
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

        this->window.onKeyDownEvent.addListener([&](const int a_keycode){this->hangarOnKeyDownCallback(a_keycode);});
        this->window.onKeyUpEvent.addListener([&](const int a_keycode){this->hangarOnKeyUpCallback(a_keycode);});
        this->window.onMouseMoveEvent.addListener([&](const int a_posX, const int a_posY, const int a_posXAbs, const int a_posYAbs)
        {
            this->hangarOnMouseMoveCallback(a_posX, a_posY, a_posXAbs, a_posYAbs);
        });
    }

    Application::~Application()
    {
        for (Layer* const l_layerPtr : this->layers)
        {
            delete l_layerPtr;
        }
    }

    void Application::hangarOnKeyDownCallback(const int32_t a_keycode)
    {
        Event* const event = new Event(EventType::keyDown);
        event->keycode = a_keycode;
        this->pendingEvents.emplace_back(event);
    }

    void Application::hangarOnKeyUpCallback(const int32_t a_keycode)
    {
        Event* const event = new Event(EventType::keyUp);
        event->keycode = a_keycode;
        this->pendingEvents.emplace_back(event);
    }

    void Application::hangarOnMouseMoveCallback(const int a_posX, const int a_posY, const int a_posXAbs, const int a_posYAbs)
    {
        Event* const event = new Event(EventType::mouseMove);
        event->mousePosition.relative = {a_posX, a_posY};
        event->mousePosition.absolute = {a_posXAbs, a_posYAbs};
        this->pendingEvents.emplace_back(event);
    }
}
