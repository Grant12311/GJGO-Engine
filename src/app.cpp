#include <GJGOpch.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/app.hpp>
#include <GJGO/display.hpp>
#include <GJGO/editor_layer.hpp>
#include <GJGO/event.hpp>
#include <GJGO/imgui_layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/scene.hpp>
#include <GJGO/scene_management_layer.hpp>
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

    Layer& App::getLayerByName(const char* const a_name)
    {
        for (unsigned short i = 0; i < this->layers.size(); i++)
        {
            if (std::strcmp(a_name, this->layers[i]->name) == 0)
            {
                return *this->layers[i];
            }
        }

        GJGO_LOG_ERROR("No layer with name ", a_name, "!");
        //This should NEVER run!!!
        return *this->layers[0];
    }

    void App::run()
    {
        GJGO_PROFILE_FUNCTION();

        double lastTime = glfwGetTime();

        this->layers.emplace_back(new ImGuiLayer);
        this->layers.emplace_back(new SceneManagementLayer);
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
            App::instance->pendingEvents.emplace_back(event);
        }else if (a_action == GLFW_RELEASE){
            Event* const event = new Event(EventType::keyUp);
            event->keycode = a_key;
            App::instance->pendingEvents.emplace_back(event);
        }
    }

    static void keyTypedCallback(GLFWwindow* const /*a_windowPtr*/, const unsigned int a_char)
    {
        GJGO_PROFILE_FUNCTION();

        Event* const event = new Event(EventType::keyTypedDown);
        event->keycode = a_char;
        App::instance->pendingEvents.emplace_back(event);
    }

    static void mousePositionCallback(GLFWwindow* const /*a_windowPtr*/, const double a_x, const double a_y)
    {
        GJGO_PROFILE_FUNCTION();

        Position2D windowPosition = Window::getPosition();
        unsigned int windowHeight = Window::getHeight();

        Event* const event = new Event(EventType::mouseMove);
        event->mousePosition.relative = {static_cast<int>(a_x), static_cast<int>(windowHeight) - static_cast<int>(a_y) - 1};

        event->mousePosition.absolute = {windowPosition.x + static_cast<int>(a_x), windowPosition.y + event->mousePosition.relative.y};

        App::instance->pendingEvents.emplace_back(event);
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
        App::instance->pendingEvents.emplace_back(event);
    }

    static void mouseWheelCallback(GLFWwindow* const /*a_window*/, const double /*a_xOffset*/, const double a_yOffset)
    {
        GJGO_PROFILE_FUNCTION();

        Event* const event = new Event(EventType::mouseWheelScroll);
        event->mouseWheelDirection = static_cast<signed char>(a_yOffset);
        App::instance->pendingEvents.emplace_back(event);
    }

    static void windowSizeCallback(GLFWwindow* const /*a_windowPtr*/, const int a_width, const int a_height)
    {
        GJGO_PROFILE_FUNCTION();

        Event* const event = new Event(EventType::windowResize);
        event->windowSize = {static_cast<unsigned int>(a_width), static_cast<unsigned int>(a_height)};
        App::instance->pendingEvents.emplace_back(event);
    }

    App::App()
    {
        #ifndef GJGO_BUILD_TARGET_DIST
            Profiler::Get().BeginSession("main");
        #endif // GJGO_BUILD_TARGET_DIST
        GJGO_PROFILE_FUNCTION();

        App::instance = this;

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

        this->currentScene = new Scene;
    }

    App::~App()
    {
        GJGO_PROFILE_FUNCTION();

        delete this->currentScene;

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

        App::instance->vsyncEnabled = a_vsync;
        glfwSwapInterval(a_vsync);
    }

    void setFramerateCap(const double a_cap)
    {
        GJGO_PROFILE_FUNCTION();

        App::instance->framerateCap = 1000.0d / a_cap;
    }
}
