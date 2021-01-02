#include <pch.hpp>

#include <GJGO/app.hpp>
#include <GJGO/editor_layer.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    void EditorLayer::drawGui()
    {
        GJGO_PROFILE_FUNCTION();

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

            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Entities", NULL, &this->showEntitiesWindow);

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if (this->showEntitiesWindow)
        {
            if (ImGui::Begin("Entities", &this->showEntitiesWindow))
            {

            }
            ImGui::End();
        }

        // Overlay
        ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs;
        ImGui::SetNextWindowPos({static_cast<float>(Window::getWidth()) - ImGui::CalcTextSize("16.667 ms/frame (60.0 FPS)").x - 10.0f, 10.0f});
        if (ImGui::Begin("Example: Simple overlay", NULL, overlayFlags))
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }
        ImGui::End();
    }
}
