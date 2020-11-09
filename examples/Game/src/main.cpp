#include <iostream>

#include <GLES3/gl31.h>

#include <imgui.h>

#include <Hangar2/keycodes.h>

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
        if (GJGO::g_appInstancePtr->window.keyIsDown(HGR_w))
            this->m_playerPosition.y += GJGO::g_appInstancePtr->window.deltaTime;
        else if (GJGO::g_appInstancePtr->window.keyIsDown(HGR_s))
            this->m_playerPosition.y -= GJGO::g_appInstancePtr->window.deltaTime;

        if (GJGO::g_appInstancePtr->window.keyIsDown(HGR_a))
            this->m_playerPosition.x -= GJGO::g_appInstancePtr->window.deltaTime;
        else if (GJGO::g_appInstancePtr->window.keyIsDown(HGR_d))
            this->m_playerPosition.x += GJGO::g_appInstancePtr->window.deltaTime;

        std::cout << GJGO::g_appInstancePtr->window.deltaTime << std::endl;
    }

    void onEvent(GJGO::Event* const a_event) override
    {
        switch (a_event->type)
        {
            case GJGO::EventType::keyDown:
                GJGO_LOG_INFO("Key Down: ", a_event->keycode);
                switch (a_event->keycode)
                {
                    case HGR_0:
                        GJGO::g_appInstancePtr->window.setVsync(0);
                        break;
                    case HGR_1:
                        GJGO::g_appInstancePtr->window.setVsync(1);
                        GJGO::g_appInstancePtr->window.setFramerateCap(60);
                        break;
                    case HGR_2:
                        GJGO::g_appInstancePtr->window.setVsync(1);
                        GJGO::g_appInstancePtr->window.setFramerateCap(30);
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
        //this->m_fbo.bind();
        //this->m_vao.bind();
        this->m_shader.bind();
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        GJGO::Renderer::genOrthoMatrix(GJGO::g_appInstancePtr->window.getWidth(), GJGO::g_appInstancePtr->window.getHeight());
        GJGO::Renderer::drawQuad(&this->m_shader, this->m_playerPosition, {100, 100});
        //this->m_fbo.unbind();
    }

    void drawGui() override
    {
        /*ImGui::Begin("Renderer", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Image((void*)this->m_fbo.colorAttachment, {100, 100}, {0, 1}, {1, 0});

        ImGui::End();*/
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
    Hangar::Config winConfig;
    winConfig.vsync = false;
    GJGO::Application app(winConfig);

    app.layers.emplace_back(new GameLayer);

    GJGO::Entity e;
    e.addComponent<GJGO::Transform2D>();

    app.run();

    return 0;
}
