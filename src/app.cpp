#include <GJGO/app.hpp>
#include <GJGO/log.hpp>

namespace GJGO
{
    static void framebufferResizeCallback(GLFWwindow* const a_window, const int a_width, const int a_height)
    {
        glViewport(0, 0, a_width, a_height);
    }

    App::App(const AppSettings &a_settings)
    {
        this->instance = this;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->window = glfwCreateWindow(a_settings.windowWidth, a_settings.windowHeight, a_settings.windowName, nullptr, nullptr);
        if (this->window == nullptr)
        {
            GJGO_LOG_ERROR("Failed to create GLFW window!");
            glfwTerminate();
        }
        glfwMakeContextCurrent(this->window);

        glfwSetFramebufferSizeCallback(this->window, framebufferResizeCallback);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            GJGO_LOG_ERROR("Failed to initialize GLAD");
        }

        GJGO_LOG_INFO(glGetString(GL_VERSION));

        glViewport(0, 0, a_settings.windowWidth, a_settings.windowHeight);
    }

    App::~App()
    {
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }

    void App::run()
    {
        while (!glfwWindowShouldClose(this->window))
        {
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(this->window);
        }
    }
}
