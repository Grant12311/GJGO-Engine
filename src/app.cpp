#include <GJGO/app.hpp>
#include <GJGO/log.hpp>

namespace GJGO
{
    App::App()
    {
        this->instance = this;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->window = glfwCreateWindow(800, 600, "GJGO", nullptr, nullptr);
        if (this->window == nullptr)
        {
            GJGO_LOG_ERROR("Failed to create GLFW window!");
            glfwTerminate();
        }
        glfwMakeContextCurrent(this->window);

        //glfwSetFramebufferSizeCallback(this->window, windowResizeCallback);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            GJGO_LOG_ERROR("Failed to initialize GLAD");
        }

        std::cout << glGetString(GL_VERSION) << std::endl;
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
