#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <vector>

#include <entt/entity/registry.hpp>

#include <GLFW/glfw3.h>

#include <GJGO/layer.hpp>
#include <GJGO/event.hpp>

namespace GJGO
{
    class Application
    {
    public:
        GLFWwindow* windowPtr;
        entt::registry registry;

        std::vector<Layer*> layers;
        std::vector<Event*> pendingEvents;

        void run();

        Application(const Hangar::Config &a_config = Hangar::Config());
        ~Application();
    };

    inline Application* g_appInstancePtr = nullptr;
}

#endif
