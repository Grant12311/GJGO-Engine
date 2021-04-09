#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <vector>

#include <entt/entity/registry.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GJGO/event.hpp>
#include <GJGO/layer.hpp>

namespace GJGO
{
    struct AppSettings
    {
        const char* windowName = "GJGO";
        int windowWidth = 800, windowHeight = 600;
    };

    class App
    {
    public:
        App(const AppSettings &a_settings);
        ~App();

        void run();

        inline static App* instance;

        GLFWwindow* window;
        entt::registry registry;

        double deltaTime;

        std::vector<Event> pendingEvents;
        std::vector<Layer*> layers;
    private:
        void drawEntities();
    };
}

#endif
