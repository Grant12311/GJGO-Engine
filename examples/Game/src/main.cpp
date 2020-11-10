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
    Druid::FBO m_fbo;
    Druid::VAO m_vao;
    Druid::VBO m_vbo;
    Druid::Shader m_shader;

    GJGO::Position2D m_playerPosition;
public:
    void onUpdate() override
    {
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
            case GJGO::EventType::mouseMove:
                GJGO_LOG_INFO("(", a_event->mousePosition.absolute.x, ", ", a_event->mousePosition.absolute.y, ")");
                break;
        }
    }

    void draw() override
    {
        this->m_fbo.bind();
        //this->m_vao.bind();
        this->m_shader.bind();
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        glClear(GL_COLOR_BUFFER_BIT);

        GJGO::Renderer::genOrthoMatrix(GJGO::Window::getWidth(), GJGO::Window::getHeight());
        GJGO::Renderer::drawQuad(&this->m_shader, this->m_playerPosition, {100, 100});

        this->m_fbo.unbind();
    }

    void drawGui() override
    {
        ImGui::Begin("Renderer", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Image((void*)this->m_fbo.colorAttachment, {100, 100}, {0, 1}, {1, 0});

        ImGui::End();

        ImGui::ShowDemoWindow(nullptr);
    }

    GameLayer() :
        m_fbo(1600, 900), m_shader("renderer.shader")
    {
        this->m_vao.bind();
        this->m_vbo.bind();

        std::array<float, 6> vertices = {
             0.0f,  0.5f,
             0.5f, -0.5f,
            -0.5f, -0.5f
        };

        this->m_vbo.fill(6 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        this->m_vao.setAttrib(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);
    }
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
