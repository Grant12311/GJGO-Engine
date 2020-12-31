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
#include <GJGO/window.hpp>
#include <GJGO/2D/camera2D.hpp>
#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/2D/transform2D.hpp>

class GameLayer : public GJGO::Layer
{
private:
    Druid::Shader m_shader = Druid::Shader("renderer.shader");
    int m_textureToDraw = 0;
    std::array<Druid::Texture2D, 24> m_textures = {
        Druid::Texture2D("res/sprites/dinos/blue/1.png",  false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/2.png",  false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/3.png",  false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/4.png",  false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/5.png",  false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/6.png",  false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/7.png",  false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/8.png",  false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/9.png",  false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/10.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/11.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/12.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/13.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/14.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/15.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/16.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/17.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/18.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/19.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/20.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/21.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/22.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/23.png", false, GL_NEAREST, GL_NEAREST),
        Druid::Texture2D("res/sprites/dinos/blue/24.png", false, GL_NEAREST, GL_NEAREST)
    };

    GJGO::Position2D m_playerPosition = {300, 10};
    GJGO::Size2D m_size = {1000, 1000};
    float m_rotation = 0.0f;

    GJGO::Camera2D m_camera;

    std::array<float, 3> m_clearColor = {
        0.0f, 0.0f, 0.0f
    };

    bool m_showRendererWindow = true;
public:
    Druid::FBO fbo = Druid::FBO(1280, 720);

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
            this->m_size.width += 10;
            this->m_size.height += 10;
        }else if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_DOWN)){
            this->m_size.width -= 10;
            this->m_size.height -= 10;
        }

        if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_RIGHT))
            this->m_rotation += 1;
        else if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_LEFT))
            this->m_rotation -= 1;

        if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_LEFT_BRACKET))
        {
            for (unsigned char i = 0; i < this->m_textures.size(); i++)
            {
                this->m_textures[i].setFilters(GL_NEAREST, GL_NEAREST);
            }
        }else if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_RIGHT_BRACKET)){
            for (unsigned char i = 0; i < this->m_textures.size(); i++)
            {
                this->m_textures[i].setFilters(GL_LINEAR, GL_LINEAR);
            }
        }

        if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_MINUS))
        {
            this->m_camera.position.z -= 1;
        }else if (glfwGetKey(GJGO::g_appInstancePtr->windowPtr, GLFW_KEY_EQUAL)){
            this->m_camera.position.z += 1;
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
                this->m_camera.position.z += 1 * a_event->mouseWheelDirection;
                break;
        }
    }

    void draw() override
    {
        GJGO_PROFILE_FUNCTION();

        glClearColor(this->m_clearColor[0], this->m_clearColor[1], this->m_clearColor[2], 1.0f);

        GJGO::Renderer::begin2D(&this->m_shader, this->m_camera, GJGO::Window::getWidth(), GJGO::Window::getHeight());

        GJGO::Renderer::drawQuad(this->m_playerPosition, this->m_size, this->m_rotation, {1.0f, 1.0f, 1.0f}, this->m_textures[this->m_textureToDraw]);
    }

    void drawGui() override
    {
        GJGO_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();

        ImGui::Begin("Anim");
        ImGui::SliderInt("Frame", &this->m_textureToDraw, 0, 23);
        ImGui::InputInt2("Pos", reinterpret_cast<int*>(&this->m_playerPosition));
        ImGui::InputInt2("Size", reinterpret_cast<int*>(&this->m_size));
        ImGui::SliderFloat("Rotation", &this->m_rotation, 0.0f, 360.0f);
        ImGui::Separator();
        ImGui::SliderFloat3("Clear Color", this->m_clearColor.data(), 0, 1.0f);
        ImGui::End();

        ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs;
        ImGui::SetNextWindowPos({static_cast<float>(GJGO::Window::getWidth()) - ImGui::CalcTextSize("16.667 ms/frame (60.0 FPS)").x - 10.0f, 10.0f});
        if (ImGui::Begin("Example: Simple overlay", NULL, overlayFlags))
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }
        ImGui::End();
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
