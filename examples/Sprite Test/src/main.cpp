#include <iostream>

#include <glm/vec2.hpp>

#include <imgui.h>

#include <tweeny.h>

#include <Druid/fbo.h>
#include <Druid/vao.h>
#include <Druid/vbo.h>
#include <Druid/ibo.h>
#include <Druid/shader.h>
#include <Druid/texture.h>

#include <GJGO/2D/camera2D.hpp>
#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/2D/transform2D.hpp>
#include <GJGO/app.hpp>
#include <GJGO/clipboard.hpp>
#include <GJGO/components.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/event.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/window.hpp>

class GameLayer : public GJGO::Layer
{
private:
    GJGO::Entity dinoEntity;
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

    int m_animChoice = 0;
    tweeny::tween<int> m_animTween = tweeny::from(2).to(8).during(700).onStep([](tweeny::tween<int> &a_tween)
    {
        if (a_tween.progress() >= 1.0f)
        {
            a_tween.seek(0);
        }

        return false;
    });

    GJGO::Camera2D m_camera;

    std::array<float, 3> m_clearColor = {
        0.0f, 0.0f, 0.0f
    };
public:
    void onUpdate() override
    {
        GJGO_PROFILE_FUNCTION();

        this->m_textureToDraw = this->m_animTween.step(static_cast<int>(GJGO::Window::deltaTime));

        GJGO::Transform2DComponent& plrTransform = this->dinoEntity.getComponentAccess<GJGO::Transform2DComponent>();

        if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_W))
            plrTransform.position.y += GJGO::Window::deltaTime;
        else if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_S))
            plrTransform.position.y -= GJGO::Window::deltaTime;

        if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_A))
            plrTransform.position.x -= GJGO::Window::deltaTime;
        else if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_D))
            plrTransform.position.x += GJGO::Window::deltaTime;

        if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_UP))
        {
            plrTransform.size.x += 10;
            plrTransform.size.y += 10;
        }else if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_DOWN)){
            plrTransform.size.x -= 10;
            plrTransform.size.y -= 10;
        }

        if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_RIGHT))
            plrTransform.rotation += 1;
        else if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_LEFT))
            plrTransform.rotation -= 1;

        if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_LEFT_BRACKET))
        {
            for (unsigned char i = 0; i < this->m_textures.size(); i++)
            {
                this->m_textures[i].setFilters(GL_NEAREST, GL_NEAREST);
            }
        }else if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_RIGHT_BRACKET)){
            for (unsigned char i = 0; i < this->m_textures.size(); i++)
            {
                this->m_textures[i].setFilters(GL_LINEAR, GL_LINEAR);
            }
        }

        if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_MINUS))
        {
            this->m_camera.position.z -= 1;
        }else if (glfwGetKey(GJGO::Application::instance->windowPtr, GLFW_KEY_EQUAL)){
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

    void drawGui() override
    {
        GJGO_PROFILE_FUNCTION();

        GJGO::Transform2DComponent& plrTransform = this->dinoEntity.getComponentAccess<GJGO::Transform2DComponent>();

        ImGuiIO& io = ImGui::GetIO();

        ImGui::Begin("Anim");
        constexpr std::array<const char*, 4> choices = {"Walk", "Jump", "Hurt", "Run"};
        ImGui::ListBox("Anims", &this->m_animChoice, choices.data(), choices.size());
        ImGui::SliderInt("Frame", &this->m_textureToDraw, 0, 23);
        ImGui::Separator();
        ImGui::InputFloat2("Pos", &plrTransform.position.x);
        ImGui::InputFloat2("Size", &plrTransform.size.x);
        ImGui::SliderFloat("Rotation", &plrTransform.rotation, 0.0f, 360.0f);
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

        glClearColor(this->m_clearColor[0], this->m_clearColor[1], this->m_clearColor[2], 1.0f);
    }

    GameLayer()
    {
        GJGO_PROFILE_FUNCTION();

        this->name = "Game";

        GJGO::Window::maximize();

        GJGO::Application::instance->currentScene->primaryCamera = &this->m_camera;
        this->dinoEntity = GJGO::Application::instance->currentScene->createEntity("Dino");
        this->dinoEntity.addComponent<GJGO::Transform2DComponent>(glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 100.0f));
        this->dinoEntity.addComponent<GJGO::SpriteComponent>(static_cast<unsigned int>(this->m_textures[0]), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
};

int main()
{
    GJGO_LOG_SET_PRINT_FILE(false);
    GJGO_LOG_SET_PRINT_FUNCTION(false);
    GJGO::Application app;

    app.layers.emplace_back(new GameLayer);

    app.run();

    return 0;
}
