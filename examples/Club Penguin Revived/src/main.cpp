#include <imgui.h>

#include <Druid/shader.h>
#include <Druid/texture.h>

#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/app.hpp>
#include <GJGO/2D/camera2D.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/window.hpp>

class GameLayer : public GJGO::Layer
{
public:
    GJGO::Camera2D camera;
    GJGO::Position2D playerPosition = {100, 100};

    Druid::Shader shader;
    Druid::Texture2D playerTexture;

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
        }
    }

    void draw() override
    {
        GJGO_PROFILE_FUNCTION();

        GJGO::Renderer::begin2D(&this->shader, this->camera, GJGO::Window::getWidth(), GJGO::Window::getHeight());

        GJGO::Renderer::drawQuad(this->playerPosition, {61, 69}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, this->playerTexture);
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
        shader("sprite.shader"), playerTexture("res/penguin/purple/down.png", false)
    {
        GJGO_PROFILE_FUNCTION();

        this->name = "Game";
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