#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    };
}

#endif
