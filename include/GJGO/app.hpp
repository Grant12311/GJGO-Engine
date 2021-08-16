#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <vector>

#include <entt/entity/registry.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GJGO/event.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/renderer2D.hpp>

namespace GJGO
{
    struct AppSettings
    {
        const char* windowName;
        int windowWidth, windowHeight;

        AppSettings(const char* const a_windowName = "GJGO", const int a_windowWidth = 800, const int a_windowHeight = 600);
    };

    class App
    {
    public:
        App(const AppSettings &a_settings = {});
        ~App();

        void run();

        inline static App* instance;

        GLFWwindow* window;
        entt::registry registry;

        double deltaTime;

        std::vector<Event> pendingEvents;
        std::vector<Layer*> layers;
    private:
        Renderer::Batch2D* m_batch;

        void drawEntities();
    };
}

#endif
