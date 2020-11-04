#include <backends/imgui_impl_opengl3.h>

#include <GJGO/app.hpp>
#include <GJGO/event.hpp>
#include <GJGO/imgui_layer.hpp>
#include <GJGO/log.hpp>

namespace GJGO
{
    void Application::run()
    {
        this->layers.emplace_back(new ImGuiLayer);

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

            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();
            for (Layer* const l_layerPtr : this->layers)
            {
                l_layerPtr->drawGui();
            }
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            this->window.swapBuffers();
        }
    }

    Application::Application(const Hangar::Config &a_config) :
        window(a_config)
    {
        GJGO_LOG_INFO(glGetString(GL_VERSION));

        this->window.onKeyDownEvent.addListener([&](const int a_keycode)
        {
            Event* const event = new Event(EventType::keyDown);
            event->keycode = a_keycode;
            this->pendingEvents.emplace_back(event);
        });
        this->window.onKeyUpEvent.addListener([&](const int a_keycode)
        {
            Event* const event = new Event(EventType::keyUp);
            event->keycode = a_keycode;
            this->pendingEvents.emplace_back(event);
        });
        this->window.onKeyTypedDownEvent.addListener([&](const int a_keycode)
        {
            Event* const event = new Event(EventType::keyTypedDown);
            event->keycode = a_keycode;
            this->pendingEvents.emplace_back(event);
        });this->window.onKeyTypedUpEvent.addListener([&](const int a_keycode)
        {
            Event* const event = new Event(EventType::keyTypedUp);
            event->keycode = a_keycode;
            this->pendingEvents.emplace_back(event);
        });

        this->window.onMouseMoveEvent.addListener([&](const int a_posX, const int a_posY, const int a_posXAbs, const int a_posYAbs)
        {
            Event* const event = new Event(EventType::mouseMove);
            event->mousePosition.relative = {a_posX, a_posY};
            event->mousePosition.absolute = {a_posXAbs, a_posYAbs};
            this->pendingEvents.emplace_back(event);
        });

        this->window.onMouseButtonDownEvent.addListener([&](const uint8_t a_mouseButton)
        {
            Event* const event = new Event(EventType::mouseButtonDown);
            event->mouseButton = a_mouseButton;
            this->pendingEvents.emplace_back(event);
        });
        this->window.onMouseButtonUpEvent.addListener([&](const uint8_t a_mouseButton)
        {
            Event* const event = new Event(EventType::mouseButtonUp);
            event->mouseButton = a_mouseButton;
            this->pendingEvents.emplace_back(event);
        });

        this->window.onResizeEvent.addListener([&](const int a_width, const int a_height)
        {
            Event* const event = new Event(EventType::windowResize);
            event->windowSize = {static_cast<uint32_t>(a_width), static_cast<uint32_t>(a_height)};
            this->pendingEvents.emplace_back(event);
        });

        g_appInstancePtr = this;
    }

    Application::~Application()
    {
        for (Layer* const l_layerPtr : this->layers)
        {
            delete l_layerPtr;
        }
    }
}
