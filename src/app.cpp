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

        glViewport(0, 0, a_settings.windowWidth, a_settings.windowHeight);

        Renderer::init2D();
    }

    App::~App()
    {
        for (Layer* const l_layer : this->layers)
        {
            delete l_layer;
        }

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

    static void drawEntityList(const std::vector<Entity> &a_entities)
    {
        Renderer::Batch2D batch;
        unsigned int texturesUsed = 0;

        for (Entity l_entity : a_entities)
        {
            const SpriteComponent& sprite = l_entity.getComponent<SpriteComponent>();

            if (std::find(batch.textures.begin(), batch.textures.end(), sprite.texture) == batch.textures.end())
            {
                batch.textures[texturesUsed++] = sprite.texture;
            }
        }

        Renderer::begin2D(*Camera2D::primary, Window::getWidth(), Window::getHeight());

        for (Entity l_entity : a_entities)
        {
            const Transform2DComponent& transform = l_entity.getComponent<Transform2DComponent>();
            const SpriteComponent& sprite = l_entity.getComponent<SpriteComponent>();

            batch.addQuad(transform.position, transform.size, transform.rotation, sprite.color, sprite.texture != nullptr ? std::distance(batch.textures.begin(), std::find(batch.textures.begin(), batch.textures.end(), sprite.texture)) : -1.0f);
        }

        batch.draw();
    }

    void App::drawEntities()
    {
        if (Renderer::useBatchRendererAsDefault)
        {
            std::vector<Entity> spriteEntities;
            std::vector<Entity> spriteEntitiesTransparent;
            spriteEntities.reserve(this->registry.size<SpriteComponent>());
            auto view = this->registry.view<Transform2DComponent, SpriteComponent>();

            for (const entt::entity l_entity : view)
            {
                const GJGO::SpriteComponent& sprite = view.get<SpriteComponent>(l_entity);

                bool textureUsesTransparency = false;

                if (sprite.texture != nullptr)
                {
                    textureUsesTransparency = (sprite.texture->getSettings() & TextureSettings::hasTransparency);
                }

                if (sprite.color.a >= 1.0f && !textureUsesTransparency)
                {
                    spriteEntities.emplace_back(l_entity);
                }else{
                    spriteEntitiesTransparent.emplace_back(l_entity);
                }
            }

            std::sort(spriteEntities.begin(), spriteEntities.end(), [](Entity a_a, Entity a_b) -> bool
            {
                return a_a.getComponent<SpriteComponent>().layer < a_b.getComponent<SpriteComponent>().layer;
            });
            std::sort(spriteEntitiesTransparent.begin(), spriteEntitiesTransparent.end(), [](Entity a_a, Entity a_b) -> bool
            {
                return a_a.getComponent<SpriteComponent>().layer < a_b.getComponent<SpriteComponent>().layer;
            });

            drawEntityList(spriteEntities);
            drawEntityList(spriteEntitiesTransparent);
        }else{
            //std::map<float, std::vector<Entity>> spriteEntities;
            std::vector<Entity> spriteEntities;
            spriteEntities.reserve(this->registry.size<SpriteComponent>());
            auto view = this->registry.view<Transform2DComponent, SpriteComponent>();

            for (const entt::entity l_entity : view)
            {
                spriteEntities.emplace_back(Entity(l_entity));
                //spriteEntities[view.get<SpriteComponent>(l_entity).layer].emplace_back(Entity(l_entity));
            }

            std::sort(spriteEntities.begin(), spriteEntities.end(), [](Entity a_a, Entity a_b) -> bool
            {
                return a_a.getComponent<SpriteComponent>().layer < a_b.getComponent<SpriteComponent>().layer;
            });

            GJGO::Renderer::begin2D(*Camera2D::primary, Window::getWidth(), Window::getHeight());

            for (const Entity l_entity : spriteEntities)
            {
                auto[transform, sprite] = view.get<Transform2DComponent, SpriteComponent>(l_entity.getRaw());

                Renderer::drawQuad(transform.position, transform.size, transform.rotation, sprite.color, sprite.texture);
            }

            /*for (const std::pair<float, std::vector<Entity>> &l_pair : spriteEntities)
            {
                for (const Entity l_entity : l_pair.second)
                {
                    auto[transform, sprite] = view.get<Transform2DComponent, SpriteComponent>(l_entity.getRaw());

                    Renderer::drawQuad(transform.position, transform.size, transform.rotation, sprite.color, sprite.texture);
                }
            }*/
        }
    }
}
