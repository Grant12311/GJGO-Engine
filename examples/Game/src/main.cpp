#include <iostream>

#include <imgui.h>

#include <Hangar2/keycodes.h>

#include <GJGO/app.hpp>
#include <GJGO/clipboard.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/event.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/position2D.hpp>

class GameLayer : public GJGO::Layer
{
public:
    void onUpdate() override
    {
        //GJGO_LOG_INFO(this->parentPtr->window.deltaTime);
    }

    void onEvent(GJGO::Event* const a_event) override
    {
        switch (a_event->type)
        {
            case GJGO::EventType::keyDown:
                GJGO_LOG_INFO("Key Down: ", a_event->keycode);
                break;
            case GJGO::EventType::keyUp:
                GJGO_LOG_INFO("Key Up: ", a_event->keycode);
                break;
            case GJGO::EventType::mouseMove:
                GJGO_LOG_INFO("(", a_event->mousePosition.absolute.x, ", ", a_event->mousePosition.absolute.y, ")");
                break;
            case GJGO::EventType::windowResize:
                GJGO_LOG_INFO("Window Size: (", a_event->windowSize.width, ", ", a_event->windowSize.height, ")");
                break;
        }
    }

    void draw() override
    {
        //GJGO_LOG_INFO("Draw!");
    }
};

class EditorLayer : public GJGO::Layer
{
public:
    void drawGui()
    {
        ImGuiIO& io = ImGui::GetIO();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New")) {}
                ImGui::Separator();

                if (ImGui::MenuItem("Open", "Ctrl+O")) {}
                if (ImGui::BeginMenu("Open Recent"))
                {
                    ImGui::MenuItem("Game");

                    ImGui::EndMenu();
                }
                ImGui::Separator();

                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save as...")) {}
                ImGui::Separator();

                if (ImGui::MenuItem("Close")) {}
                ImGui::Separator();

                if (ImGui::MenuItem("Quit", "Ctrl+Q")) {}

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Overlay
        ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs;
        ImGui::SetNextWindowPos({GJGO::g_appInstancePtr->window.width - ImGui::CalcTextSize("16.667 ms/frame (60.0 FPS)").x - 10, 10.0f});
        if (ImGui::Begin("Example: Simple overlay", NULL, overlayFlags))
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::End();
        }
    }
};

int main()
{
    GJGO_LOG_SET_PRINT_FILE(false);
    GJGO_LOG_SET_PRINT_FUNCTION(false);
    GJGO::Application app;

    app.layers.emplace_back(new GameLayer);
    #ifndef GJGO_BUILD_TARGET_DIST
        app.layers.emplace_back(new EditorLayer);
    #endif // GJGO_BUILD_TARGET_DIST

    GJGO::Entity e(&app);
    e.addComponent<GJGO::Position2D>(5, 5);
    e.editComponent<GJGO::Position2D>(0, 90);

    GJGO_LOG_INFO("(", e.getComponent<GJGO::Position2D>().x, ", ", e.getComponent<GJGO::Position2D>().y, ")");

    app.run();

    return 0;
}
