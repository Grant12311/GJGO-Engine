#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GJGO
{
    class App
    {
    public:
        App();
        ~App();

        void run();

        inline static App* instance;

        GLFWwindow* window;
    };
}

#endif
