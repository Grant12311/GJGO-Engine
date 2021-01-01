#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <backends/imgui_impl_opengl3.h>

#include <GJGO/app.hpp>
#include <GJGO/editor_layer.hpp>
#include <GJGO/event.hpp>
#include <GJGO/imgui_layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    static void openglDebugLogger(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
    {
        GJGO_PROFILE_FUNCTION();

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

    Layer& Application::getLayerByName(const char* const a_name)
    {
        for (unsigned short i = 0; i < this->layers.size(); i++)
        {
            if (std::strcmp(a_name, this->layers[i]->name) == 0)
            {
                return *this->layers[i];
            }
        }
    }

    void Application::run()
    {
        GJGO_PROFILE_FUNCTION();

        double lastTime = glfwGetTime();

        this->layers.emplace_back(new ImGuiLayer);
        #ifndef GJGO_BUILD_TARGET_DIST
            //this->layers.emplace_back(new EditorLayer);
        #endif // GJGO_BUILD_TARGET_DIST

        while (!glfwWindowShouldClose(this->windowPtr))
        {
            glfwPollEvents();

            {
                GJGO_PROFILE_SCOPE("Update");

                for (Layer* const l_layerPtr : this->layers)
                {
                    l_layerPtr->onUpdate();
                }
            }

            {
                GJGO_PROFILE_SCOPE("Event Dispatch");

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
            }

            {
                GJGO_PROFILE_SCOPE("Draw");

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                for (Layer* const l_layerPtr : this->layers)
                {
                    l_layerPtr->draw();
                }
            }

            {
                GJGO_PROFILE_SCOPE("ImGui");

                ImGui_ImplOpenGL3_NewFrame();
                ImGui::NewFrame();
                for (Layer* const l_layerPtr : this->layers)
                {
                    l_layerPtr->drawGui();
                }
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }

            {
                GJGO_PROFILE_SCOPE("VSYNC");

                {
                    GJGO_PROFILE_SCOPE("glfwSwapBuffers");

                    glfwSwapBuffers(this->windowPtr);
                }

                {
                    GJGO_PROFILE_SCOPE("Timer");

                    if (this->vsyncEnabled)
                        while ((glfwGetTime() - lastTime) * 1000.0d < this->framerateCap) {}

                    Window::deltaTime = (glfwGetTime() - lastTime) * 1000.0d;

                    lastTime = glfwGetTime();
                }
            }
        }
    }

    static void error_callback(int /*error*/, const char* description)
    {
        GJGO_PROFILE_FUNCTION();

        fprintf(stderr, "Error: %s\n", description);
    }

    static void keyCallback(GLFWwindow* const /*a_windowPtr*/, const int a_key, const int /*a_scancode*/, const int a_action, const int /*a_mods*/)
    {
        GJGO_PROFILE_FUNCTION();

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

    static void keyTypedCallback(GLFWwindow* const /*a_windowPtr*/, const unsigned int a_char)
    {
        GJGO_PROFILE_FUNCTION();

        Event* const event = new Event(EventType::keyTypedDown);
        event->keycode = a_char;
        g_appInstancePtr->pendingEvents.emplace_back(event);
    }

    static void mousePositionCallback(GLFWwindow* const /*a_windowPtr*/, const double a_x, const double a_y)
    {
        GJGO_PROFILE_FUNCTION();

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

    static void mouseButtonCallback(GLFWwindow* const /*a_windowPtr*/, const int a_button, const int a_action, const int /*a_mods*/)
    {
        GJGO_PROFILE_FUNCTION();

        Event* event;

        if (a_action == GLFW_PRESS)
        {
            event = new Event(EventType::mouseButtonDown);
        }else{
            event = new Event(EventType::mouseButtonUp);
        }

        event->mouseButton = static_cast<unsigned char>(a_button);
        g_appInstancePtr->pendingEvents.emplace_back(event);
    }

    static void mouseWheelCallback(GLFWwindow* const /*a_window*/, const double /*a_xOffset*/, const double a_yOffset)
    {
        GJGO_PROFILE_FUNCTION();

        Event* const event = new Event(EventType::mouseWheelScroll);
        event->mouseWheelDirection = static_cast<signed char>(a_yOffset);
        g_appInstancePtr->pendingEvents.emplace_back(event);
    }

    static void windowSizeCallback(GLFWwindow* const /*a_windowPtr*/, const int a_width, const int a_height)
    {
        GJGO_PROFILE_FUNCTION();

        Event* const event = new Event(EventType::windowResize);
        event->windowSize = {static_cast<unsigned int>(a_width), static_cast<unsigned int>(a_height)};
        g_appInstancePtr->pendingEvents.emplace_back(event);
    }

    Application::Application()
    {
        #ifndef GJGO_BUILD_TARGET_DIST
            Profiler::Get().BeginSession("main");
        #endif // GJGO_BUILD_TARGET_DIST
        GJGO_PROFILE_FUNCTION();

        g_appInstancePtr = this;

        if (!glfwInit())
            exit(1);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

        this->windowPtr = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
        if (!this->windowPtr)
        {
            glfwTerminate();
            exit(1);
        }

        glfwMakeContextCurrent(this->windowPtr);
        glfwSwapInterval(1);

        glfwSetErrorCallback(error_callback);
        glfwSetKeyCallback(this->windowPtr, keyCallback);
        glfwSetCharCallback(this->windowPtr, keyTypedCallback);
        glfwSetCursorPosCallback(this->windowPtr, mousePositionCallback);
        glfwSetMouseButtonCallback(this->windowPtr, mouseButtonCallback);
        glfwSetScrollCallback(this->windowPtr, mouseWheelCallback);
        glfwSetWindowSizeCallback(this->windowPtr, windowSizeCallback);

        GJGO_LOG_INFO(glGetString(GL_VERSION));

        if (glfwExtensionSupported("GL_KHR_debug"))
        {
            glEnable(GL_DEBUG_OUTPUT_KHR);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);
            PFNGLDEBUGMESSAGECALLBACKKHRPROC glDebugMessageCallback = reinterpret_cast<PFNGLDEBUGMESSAGECALLBACKKHRPROC>(glfwGetProcAddress("glDebugMessageCallback"));
            glDebugMessageCallback(openglDebugLogger, 0);
        }else{
            GJGO_LOG_WARN("GL_KHR_debug extension NOT supported, OpenGL debugging DISABLED!");
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Renderer::init2D();
    }

    Application::~Application()
    {
        GJGO_PROFILE_FUNCTION();

        for (Layer* const l_layerPtr : this->layers)
        {
            delete l_layerPtr;
        }

        glfwDestroyWindow(this->windowPtr);
        glfwTerminate();

        #ifndef GJGO_BUILD_TARGET_DIST
            Profiler::Get().EndSession();
        #endif // GJGO_BUILD_TARGET_DIST
    }

    void setVsync(const bool a_vsync)
    {
        GJGO_PROFILE_FUNCTION();

        g_appInstancePtr->vsyncEnabled = a_vsync;
        glfwSwapInterval(a_vsync);
    }

    void setFramerateCap(const double a_cap)
    {
        GJGO_PROFILE_FUNCTION();

        g_appInstancePtr->framerateCap = 1000.0d / a_cap;
    }
}
