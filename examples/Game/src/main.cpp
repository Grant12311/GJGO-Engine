#include <iostream>

#include <imgui.h>

#include <Druid/fbo.h>
#include <Druid/vao.h>
#include <Druid/vbo.h>
#include <Druid/ibo.h>
#include <Druid/shader.h>
#include <Druid/texture.h>

#include <GJGO/app.hpp>
#include <GJGO/clipboard.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/event.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/2D/transform2D.hpp>
#include <GJGO/window.hpp>

class GameLayer : public GJGO::Layer
{
private:
    Druid::Shader m_shader;
    Druid::Texture2D m_texture;

    GJGO::Position2D m_playerPosition;
    GJGO::Size2D m_size;
    float m_rotation = 0.0f;

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

        if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_UP))
        {
            this->m_size.width += 1;
            this->m_size.height += 1;
        }else if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_DOWN)){
            this->m_size.width -= 1;
            this->m_size.height -= 1;
        }

        if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_RIGHT))
            this->m_rotation += 1;
        else if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_LEFT))
            this->m_rotation -= 1;

        if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_LEFT_BRACKET))
        {
            this->m_texture.setFilters(GL_NEAREST, GL_NEAREST);
        }else if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_RIGHT_BRACKET)){
            this->m_texture.setFilters(GL_LINEAR, GL_LINEAR);
        }
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

        GJGO::Renderer::begin2D(&this->m_shader, GJGO::Window::getWidth(), GJGO::Window::getHeight());

        GJGO::Renderer::drawQuad(this->m_playerPosition, this->m_size, this->m_rotation, {1.0f, 1.0f, 1.0f}, this->m_texture);
    }

    void drawGui() override
    {
        GJGO_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();

        ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs;
        ImGui::SetNextWindowPos({static_cast<float>(GJGO::Window::getWidth()) - ImGui::CalcTextSize("16.667 ms/frame (60.0 FPS)").x - 10.0f, 10.0f});
        if (ImGui::Begin("Example: Simple overlay", NULL, overlayFlags))
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }
        ImGui::End();
    }

    GameLayer() :
        fbo(1280, 720), m_shader("renderer.shader"), m_texture("wall.jpg"), m_size{100, 100} {}
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
