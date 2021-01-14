#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <vector>

#include <entt/entity/registry.hpp>

#include <GLFW/glfw3.h>

#include <GJGO/animation.hpp>
#include <GJGO/event.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/scene.hpp>

namespace GJGO
{
    class Application
    {
    public:
        static inline Application* instance;

        GLFWwindow* windowPtr;

        Scene* currentScene;
        std::vector<Layer*> layers;
        std::vector<Event*> pendingEvents;

        bool vsyncEnabled = true;
        double framerateCap = 1000.0d / 60.0d;

        Layer& getLayerByName(const char* const a_name);

        void run();

        Application();
        ~Application();
    };

    void setVsync(const bool a_vsync);
    void setFramerateCap(const double a_cap);
}

#endif
