#include <GLFW/glfw3.h>

#include <backends/imgui_impl_opengl3.h>

#include <GJGO/app.hpp>
#include <GJGO/editor_layer.hpp>
#include <GJGO/event.hpp>
#include <GJGO/imgui_layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    static void openglDebugLogger(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH_KHR:
                GJGO_LOG_ERROR("HIGH: ", message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM_KHR:
                GJGO_LOG_WARN("Med: ", message);
                break;
            case GL_DEBUG_SEVERITY_LOW_KHR:
                GJGO_LOG_WARN("Low: ", message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION_KHR:
                GJGO_LOG_INFO("Info: ", message);
                break;
        }
    }

    void Application::run()
    {
        double lastTime = glfwGetTime();

        this->layers.emplace_back(new ImGuiLayer);
        #ifndef GJGO_BUILD_TARGET_DIST
            this->layers.emplace_back(new EditorLayer);
        #endif // GJGO_BUILD_TARGET_DIST

        while (!glfwWindowShouldClose(this->windowPtr))
        {
            glfwPollEvents();

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

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

            glfwSwapBuffers(this->windowPtr);

            if (this->vsyncEnabled)
                while ((glfwGetTime() - lastTime) * 1000.0d < this->framerateCap) {}

            Window::deltaTime = (glfwGetTime() - lastTime) * 1000.0d;

            lastTime = glfwGetTime();
        }
    }

    static void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    static void keyCallback(GLFWwindow* const a_windowPtr, const int a_key, const int a_scancode, const int a_action, const int a_mods)
    {
        if (a_action == GLFW_PRESS)
        {
            Event* const event = new Event(EventType::keyDown);
            event->keycode = a_key;
            g_appInstancePtr->pendingEvents.emplace_back(event);
        }else if (a_action == GLFW_RELEASE){
            Event* const event = new Event(EventType::keyUp);
            event->keycode = a_key;
            g_appInstancePtr->pendingEvents.emplace_back(event);
        }
    }

    static void mousePositionCallback(GLFWwindow* const a_windowPtr, const double a_x, const double a_y)
    {
        int windowPosX;
        int windowPosY;
        glfwGetWindowPos(g_appInstancePtr->windowPtr, &windowPosX, &windowPosY);

        int windowWidth;
        int windowHeight;
        glfwGetWindowSize(GJGO::g_appInstancePtr->windowPtr, &windowWidth, &windowHeight);

        int screenHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

        windowPosY += windowHeight;
        windowPosY = screenHeight - windowPosY;

        Event* const event = new Event(EventType::mouseMove);
        event->mousePosition.relative = {static_cast<int>(a_x), windowHeight - static_cast<int>(a_y) - 1};

        event->mousePosition.absolute = {windowPosX + static_cast<int>(a_x), windowPosY + event->mousePosition.relative.y};

        g_appInstancePtr->pendingEvents.emplace_back(event);
    }

    static void mouseButtonCallback(GLFWwindow* const a_windowPtr, const int a_button, const int a_action, const int a_mods)
    {
        Event* event;

        if (a_action == GLFW_PRESS)
        {
            event = new Event(EventType::mouseButtonDown);
        }else{
            event = new Event(EventType::mouseButtonUp);
        }

        event->mouseButton = a_button;
        g_appInstancePtr->pendingEvents.emplace_back(event);
    }

    static void windowSizeCallback(GLFWwindow* const a_windowPtr, const int a_width, const int a_height)
    {
        Event* const event = new Event(EventType::windowResize);
        event->windowSize = {static_cast<unsigned int>(a_width), static_cast<unsigned int>(a_height)};
        g_appInstancePtr->pendingEvents.emplace_back(event);
    }

    Application::Application()
    {
        g_appInstancePtr = this;

        if (!glfwInit())
            exit(1);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

        this->windowPtr = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!this->windowPtr)
        {
            glfwTerminate();
            exit(1);
        }

        glfwMakeContextCurrent(this->windowPtr);

        glfwSetErrorCallback(error_callback);
        glfwSetKeyCallback(this->windowPtr, keyCallback);
        glfwSetCursorPosCallback(this->windowPtr, mousePositionCallback);
        glfwSetMouseButtonCallback(this->windowPtr, mouseButtonCallback);
        glfwSetWindowSizeCallback(this->windowPtr, windowSizeCallback);

        GJGO_LOG_INFO(glGetString(GL_VERSION));

        glEnable(GL_DEBUG_OUTPUT_KHR);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);
        PFNGLDEBUGMESSAGECALLBACKKHRPROC glDebugMessageCallback = reinterpret_cast<PFNGLDEBUGMESSAGECALLBACKKHRPROC>(glfwGetProcAddress("glDebugMessageCallback"));
        glDebugMessageCallback(openglDebugLogger, 0);

        /*
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
        });*/

        Renderer::init2D();
    }

    Application::~Application()
    {
        for (Layer* const l_layerPtr : this->layers)
        {
            delete l_layerPtr;
        }

        glfwDestroyWindow(this->windowPtr);
        glfwTerminate();
    }

    void setVsync(const bool a_vsync)
    {
        g_appInstancePtr->vsyncEnabled = a_vsync;
        glfwSwapInterval(a_vsync);
    }

    void setFramerateCap(const double a_cap)
    {
        g_appInstancePtr->framerateCap = 1000.0d / a_cap;
    }
}
