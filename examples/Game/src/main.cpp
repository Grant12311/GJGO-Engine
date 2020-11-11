#include <iostream>

#include <imgui.h>

#include <Druid/fbo.h>
#include <Druid/vao.h>
#include <Druid/vbo.h>
#include <Druid/ibo.h>
#include <Druid/shader.h>

#include <GJGO/app.hpp>
#include <GJGO/clipboard.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/event.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/2D/transform2D.hpp>
#include <GJGO/window.hpp>

class GameLayer : public GJGO::Layer
{
private:
    Druid::Shader m_shader;

    GJGO::Position2D m_playerPosition;

    bool m_showRendererWindow = true;
public:
    Druid::FBO fbo;

    void onUpdate() override
    {
        GJGO_PROFILE_FUNCTION();

        if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_W))
            this->m_playerPosition.y += GJGO::Window::deltaTime;
        else if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_S))
            this->m_playerPosition.y -= GJGO::Window::deltaTime;

        if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_A))
            this->m_playerPosition.x -= GJGO::Window::deltaTime;
        else if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_D))
            this->m_playerPosition.x += GJGO::Window::deltaTime;
    }

    void onEvent(GJGO::Event* const a_event) override
    {
        GJGO_PROFILE_FUNCTION();

        switch (a_event->type)
        {
            case GJGO::EventType::keyDown:
                GJGO_LOG_INFO("Key Down: ", a_event->keycode);
                switch (a_event->keycode)
                {
                    case GLFW_KEY_0:
                        GJGO::setVsync(false);
                        break;
                    case GLFW_KEY_1:
                        GJGO::setVsync(true);
                        GJGO::setFramerateCap(60);
                        break;
                    case GLFW_KEY_2:
                        GJGO::setVsync(true);
                        GJGO::setFramerateCap(30);
                        break;
                }
                break;
            case GJGO::EventType::keyUp:
                GJGO_LOG_INFO("Key Up: ", a_event->keycode);
                break;
            case GJGO::EventType::mouseWheelScroll:
                GJGO_LOG_INFO("Mouse Wheel Scroll: ", static_cast<short>(a_event->mouseWheelDirection));
                break;
        }
    }

    void draw() override
    {
        GJGO_PROFILE_FUNCTION();

        if (this->m_showRendererWindow)
        {
            this->fbo.bind();

            glClear(GL_COLOR_BUFFER_BIT);

            GJGO::Renderer::begin2D(&this->m_shader, 1600, 900);

            GJGO::Renderer::drawQuad(&this->m_shader, this->m_playerPosition, {100, 100});

            this->fbo.unbind();
        }
    }

    void drawGui() override
    {
        GJGO_PROFILE_FUNCTION();

        ImGui::ShowDemoWindow(nullptr);

        if (this->m_showRendererWindow)
        {
            ImGui::Begin("Renderer", &this->m_showRendererWindow);

            ImGui::Image((void*)this->fbo.colorAttachment, {100, 100}, {0, 1}, {1, 0});

            ImGui::End();
        }
    }

    GameLayer() :
        fbo(1600, 900), m_shader("renderer.shader") {}
};

int main()
{
    GJGO_LOG_SET_PRINT_FILE(false);
    GJGO_LOG_SET_PRINT_FUNCTION(false);
    GJGO::Application app;

    app.layers.emplace_back(new GameLayer);

    GJGO::Entity e;
    e.addComponent<GJGO::Transform2D>();

    app.run();

    return 0;
}
