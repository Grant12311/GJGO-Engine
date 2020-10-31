#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>

#include <Hangar2/keycodes.h>

#include <GJGO/app.hpp>
#include <GJGO/log.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/event.hpp>
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

    GameLayer(GJGO::Application* const a_appPtr) :
        Layer(a_appPtr) {}
};

class ImguiLayer : public GJGO::Layer
{
public:
    void draw() override
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(this->parentPtr->window.width, this->parentPtr->window.height);
        io.DeltaTime = this->parentPtr->window.deltaTime;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    ImguiLayer(GJGO::Application* const a_appPtr) :
        Layer(a_appPtr)
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.KeyMap[ImGuiKey_Tab] = HGR_tab;
        io.KeyMap[ImGuiKey_LeftArrow] = HGR_left;
        io.KeyMap[ImGuiKey_RightArrow] = HGR_right;
        io.KeyMap[ImGuiKey_UpArrow] = HGR_up;
        io.KeyMap[ImGuiKey_DownArrow] = HGR_down;
        //io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        //io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        //io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        //io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        //io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = HGR_delete;
        io.KeyMap[ImGuiKey_Backspace] = HGR_backspace;
        io.KeyMap[ImGuiKey_Space] = HGR_space;
        io.KeyMap[ImGuiKey_Enter] = HGR_enter;
        io.KeyMap[ImGuiKey_Escape] = HGR_escape;
        //io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = HGR_a;
        io.KeyMap[ImGuiKey_C] = HGR_c;
        io.KeyMap[ImGuiKey_V] = HGR_v;
        io.KeyMap[ImGuiKey_X] = HGR_x;
        io.KeyMap[ImGuiKey_Y] = HGR_y;
        io.KeyMap[ImGuiKey_Z] = HGR_z;

        ImGui_ImplOpenGL3_Init("#version 300 es");
    }
};

int main()
{
    GJGO_LOG_SET_PRINT_FILE(false);
    GJGO_LOG_SET_PRINT_FUNCTION(false);
    GJGO::Application app;

    app.layers.emplace_back(new GameLayer(&app));
    app.layers.emplace_back(new ImguiLayer(&app));

    GJGO::Entity e(&app);
    e.addComponent<GJGO::Position2D>(5, 5);
    e.editComponent<GJGO::Position2D>(0, 90);

    GJGO_LOG_INFO("(", e.getComponent<GJGO::Position2D>().x, ", ", e.getComponent<GJGO::Position2D>().y, ")");

    app.run();

    return 0;
}
