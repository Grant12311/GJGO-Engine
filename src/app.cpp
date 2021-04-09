#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <GJGO/gjgo.hpp>

namespace GJGO
{
    static void framebufferResizeCallback(GLFWwindow* const a_window, const int a_width, const int a_height)
    {
        glViewport(0, 0, a_width, a_height);
    }

    static void windowSizeCallback(GLFWwindow* const /*a_windowPtr*/, const int a_width, const int a_height)
    {
        Event& event = App::instance->pendingEvents.emplace_back(EventType::windowResize);
        event.windowSize = {a_width, a_height};
    }

    static void keyCallback(GLFWwindow* const /*a_windowPtr*/, const int a_key, const int /*a_scancode*/, const int a_action, const int /*a_mods*/)
    {
        EventType type = EventType::none;

        if (a_action == GLFW_PRESS)
            type = EventType::keyDown;
        else if (a_action == GLFW_RELEASE)
            type = EventType::keyUp;

        if (type != EventType::none)
        {
            Event& event = App::instance->pendingEvents.emplace_back(type);
            event.keycode = a_key;
        }
    }

    static void keyTypedCallback(GLFWwindow* const /*a_windowPtr*/, const unsigned int a_char)
    {
        Event& event = App::instance->pendingEvents.emplace_back(EventType::keyTypedDown);
        event.keycode = a_char;
    }

    static void mousePositionCallback(GLFWwindow* const /*a_windowPtr*/, const double a_x, const double a_y)
    {
        glm::vec2 windowPosition = Window::getPosition();
        int windowHeight = Window::getHeight();

        Event& event = App::instance->pendingEvents.emplace_back(EventType::mouseMove);
        event.mousePosition.relative = {static_cast<int>(a_x), static_cast<int>(windowHeight) - static_cast<int>(a_y) - 1};

        event.mousePosition.absolute = {windowPosition.x + static_cast<int>(a_x), windowPosition.y + event.mousePosition.relative.y};
    }

    static void mouseButtonCallback(GLFWwindow* const /*a_windowPtr*/, const int a_button, const int a_action, const int /*a_mods*/)
    {
        EventType type;

        if (a_action == GLFW_PRESS)
            type = EventType::mouseButtonDown;
        else if (a_action == GLFW_RELEASE)
            type = EventType::mouseButtonUp;

        Event& event = App::instance->pendingEvents.emplace_back(type);
        event.mouseButton = static_cast<unsigned char>(a_button);
    }

    static void mouseWheelCallback(GLFWwindow* const /*a_window*/, const double /*a_xOffset*/, const double a_yOffset)
    {
        Event& event = App::instance->pendingEvents.emplace_back(EventType::mouseWheelScroll);
        event.mouseWheelDirection = static_cast<signed char>(a_yOffset);
    }

    static void openglDebugLogger(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH_KHR:
                GJGO_LOG_ERROR("OpenGL HIGH: ", message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM_KHR:
                GJGO_LOG_WARN("OpenGL Med: ", message);
                break;
            case GL_DEBUG_SEVERITY_LOW_KHR:
                GJGO_LOG_WARN("OpenGL Low: ", message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION_KHR:
                GJGO_LOG_INFO("OpenGL Info: ", message);
                break;
        }
    }

    App::App(const AppSettings &a_settings) :
        deltaTime(0.0)
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

        glfwSwapInterval(0);

        glfwSetFramebufferSizeCallback(this->window, framebufferResizeCallback);
        glfwSetWindowSizeCallback(this->window, windowSizeCallback);
        glfwSetKeyCallback(this->window, keyCallback);
        glfwSetCharCallback(this->window, keyTypedCallback);
        glfwSetCursorPosCallback(this->window, mousePositionCallback);
        glfwSetMouseButtonCallback(this->window, mouseButtonCallback);
        glfwSetScrollCallback(this->window, mouseWheelCallback);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            GJGO_LOG_ERROR("Failed to initialize GLAD");
        }

        GJGO_LOG_INFO(glGetString(GL_VERSION));

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(openglDebugLogger, 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, a_settings.windowWidth, a_settings.windowHeight);

        Renderer::init2D();

        this->m_batch = new Renderer::Batch2D;
    }

    App::~App()
    {
        for (Layer* const l_layer : this->layers)
        {
            delete l_layer;
        }

        delete this->m_batch;

        Renderer::shutdown2D();

        glfwDestroyWindow(this->window);
        glfwTerminate();
    }

    void App::run()
    {
        double lastTime = glfwGetTime();

        while (!glfwWindowShouldClose(this->window))
        {
            glfwPollEvents();

            for (Layer* const l_layerPtr : this->layers)
            {
                l_layerPtr->onUpdate();
            }

            for (const Event &l_event : this->pendingEvents)
            {
                for (auto it = this->layers.rbegin(); it != this->layers.rend(); it++)
                {
                    (*it)->onEvent(l_event);
                    if (l_event.handled)
                        break;
                }
            }
            this->pendingEvents.clear();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            Renderer::numDrawCallsPerFrame = 0;

            this->drawEntities();

            for (Layer* const l_layerPtr : this->layers)
            {
                l_layerPtr->draw();
            }

            glfwSwapBuffers(this->window);

            this->deltaTime = (glfwGetTime() - lastTime) * 1000.0d;
            lastTime = glfwGetTime();
        }
    }

    void App::drawEntities()
    {
        this->m_batch->clear();
        unsigned int texturesUsed = 0;

        if (Renderer::useBatchRendererAsDefault)
        {
            std::vector<Entity> transparentEntities;

            auto view = this->registry.view<Transform2DComponent, SpriteComponent>();

            GJGO::Renderer::begin2D(*Camera2D::primary, Window::getWidth(), Window::getHeight());

            for (const entt::entity l_entity : view)
            {
                const Transform2DComponent& transform = view.get<Transform2DComponent>(l_entity);
                const SpriteComponent& sprite = view.get<SpriteComponent>(l_entity);

                bool textureUsesTransparency = false;

                if (sprite.texture != nullptr)
                {
                    textureUsesTransparency = (sprite.texture->getSettings() & TextureSettings::hasTransparency);
                }

                if (sprite.color.a < 1.0f || textureUsesTransparency)
                {
                    transparentEntities.emplace_back(l_entity);
                    continue;
                }

                if (std::find(this->m_batch->textures.begin(), this->m_batch->textures.end(), sprite.texture) == this->m_batch->textures.end())
                {
                    this->m_batch->textures[texturesUsed++] = sprite.texture;
                }

                this->m_batch->addQuad(transform.position, transform.size, transform.rotation, sprite.color, sprite.texture != nullptr ? std::distance(this->m_batch->textures.begin(), std::find(this->m_batch->textures.begin(), this->m_batch->textures.end(), sprite.texture)) : -1.0f);
            }

            this->m_batch->draw();

            if (transparentEntities.size() > 0)
            {
                this->m_batch->clear();
                texturesUsed = 0;

                std::sort(transparentEntities.begin(), transparentEntities.end(), [](Entity a_a, Entity a_b) -> bool
                {
                    return a_a.getComponent<Transform2DComponent>().position.z < a_b.getComponent<Transform2DComponent>().position.z;
                });

                for (Entity l_entity : transparentEntities)
                {
                    const Transform2DComponent& transform = l_entity.getComponent<Transform2DComponent>();
                    const SpriteComponent& sprite = l_entity.getComponent<SpriteComponent>();

                    if (std::find(this->m_batch->textures.begin(), this->m_batch->textures.end(), sprite.texture) == this->m_batch->textures.end())
                    {
                        this->m_batch->textures[texturesUsed++] = sprite.texture;
                    }

                    this->m_batch->addQuad(transform.position, transform.size, transform.rotation, sprite.color, sprite.texture != nullptr ? std::distance(this->m_batch->textures.begin(), std::find(this->m_batch->textures.begin(), this->m_batch->textures.end(), sprite.texture)) : -1.0f);
                }

                this->m_batch->draw();
            }
        }else{
            std::vector<Entity> transparentEntities;

            auto view = this->registry.view<Transform2DComponent, SpriteComponent>();

            GJGO::Renderer::begin2D(*Camera2D::primary, Window::getWidth(), Window::getHeight());

            for (const entt::entity l_entity : view)
            {
                auto[transform, sprite] = view.get<Transform2DComponent, SpriteComponent>(l_entity);

                bool textureUsesTransparency = false;

                if (sprite.texture != nullptr)
                {
                    textureUsesTransparency = (sprite.texture->getSettings() & TextureSettings::hasTransparency);
                }

                if (sprite.color.a < 1.0f || textureUsesTransparency)
                {
                    transparentEntities.emplace_back(l_entity);
                    continue;
                }

                Renderer::drawQuad(transform.position, transform.size, transform.rotation, sprite.color, sprite.texture);
            }

            if (transparentEntities.size() > 0)
            {
                std::sort(transparentEntities.begin(), transparentEntities.end(), [](Entity a_a, Entity a_b) -> bool
                {
                    return a_a.getComponent<Transform2DComponent>().position.z < a_b.getComponent<Transform2DComponent>().position.z;
                });

                for (Entity l_entity : transparentEntities)
                {
                    const Transform2DComponent& transform = l_entity.getComponent<Transform2DComponent>();
                    const SpriteComponent& sprite = l_entity.getComponent<SpriteComponent>();

                    Renderer::drawQuad(transform.position, transform.size, transform.rotation, sprite.color, sprite.texture);
                }
            }
        }
    }
}
