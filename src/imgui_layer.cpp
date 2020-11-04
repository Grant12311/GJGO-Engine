#include <backends/imgui_impl_opengl3.h>

#include <GJGO/app.hpp>
#include <GJGO/clipboard.hpp>
#include <GJGO/imgui_layer.hpp>

namespace GJGO
{
    static void ImGuiClipboardReadTextCallback(void* const a_userData, const char* const a_text)
    {
        GJGO::Clipboard::writeText(a_text);
    }

    static const char* ImGuiClipboardGetTextCallback(void* const a_userData)
    {
        return GJGO::Clipboard::readText();
    }

    void ImGuiLayer::onUpdate()
    {
        this->m_ioPtr->KeyCtrl = g_appInstancePtr->window.keyIsDown(HGR_control_left) || g_appInstancePtr->window.keyIsDown(HGR_control_right);
        this->m_ioPtr->KeyShift = g_appInstancePtr->window.keyIsDown(HGR_shift_left) || g_appInstancePtr->window.keyIsDown(HGR_shift_right);
        this->m_ioPtr->KeyAlt = g_appInstancePtr->window.keyIsDown(HGR_alt_left) || g_appInstancePtr->window.keyIsDown(HGR_alt_right);
        this->m_ioPtr->KeySuper = g_appInstancePtr->window.keyIsDown(HGR_super);
    }

    void ImGuiLayer::onEvent(GJGO::Event* const a_eventPtr)
    {
        switch (a_eventPtr->type)
        {
            case GJGO::EventType::keyDown:
                this->m_ioPtr->KeysDown[a_eventPtr->keycode] = true;
                break;
            case GJGO::EventType::keyUp:
                this->m_ioPtr->KeysDown[a_eventPtr->keycode] = false;
                break;
            case GJGO::EventType::keyTypedDown:
                if (std::find(this->noDrawKeycodes.begin(), this->noDrawKeycodes.end(), a_eventPtr->keycode) == this->noDrawKeycodes.end())
                    this->m_ioPtr->AddInputCharacter(static_cast<unsigned short>(a_eventPtr->keycode));
                this->m_ioPtr->KeysDown[a_eventPtr->keycode] = true;
                break;
            case GJGO::EventType::keyTypedUp:
                this->m_ioPtr->KeysDown[a_eventPtr->keycode] = false;
                break;
            case GJGO::EventType::mouseMove:
                this->m_ioPtr->MousePos = ImVec2(static_cast<float>(a_eventPtr->mousePosition.relative.x), static_cast<float>(g_appInstancePtr->window.height - a_eventPtr->mousePosition.relative.y));
                break;
            case GJGO::EventType::mouseButtonDown:
                switch (a_eventPtr->mouseButton)
                {
                    case 0:
                        this->m_ioPtr->MouseDown[0] = true;
                        break;
                    case 1:
                        this->m_ioPtr->MouseDown[2] = true;
                        break;
                    case 2:
                        this->m_ioPtr->MouseDown[1] = true;
                        break;
                }
                break;
            case GJGO::EventType::mouseButtonUp:
                switch (a_eventPtr->mouseButton)
                {
                    case 0:
                        this->m_ioPtr->MouseDown[0] = false;
                        break;
                    case 1:
                        this->m_ioPtr->MouseDown[2] = false;
                        break;
                    case 2:
                        this->m_ioPtr->MouseDown[1] = false;
                        break;
                }
                break;
            case GJGO::EventType::windowResize:
                this->m_ioPtr->DisplaySize = ImVec2(static_cast<float>(a_eventPtr->windowSize.width), static_cast<float>(a_eventPtr->windowSize.height));
                break;
        }
    }

    void ImGuiLayer::drawGui()
    {
        this->m_ioPtr->DisplaySize = ImVec2(static_cast<float>(g_appInstancePtr->window.width), static_cast<float>(g_appInstancePtr->window.height));
        this->m_ioPtr->DeltaTime = static_cast<float>(g_appInstancePtr->window.deltaTime) / 1000.0f;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

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
        ImGui::SetNextWindowPos({g_appInstancePtr->window.width - ImGui::CalcTextSize("16.667 ms/frame (60.0 FPS)").x - 10, 10.0f});
        if (ImGui::Begin("Example: Simple overlay", NULL, overlayFlags))
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / this->m_ioPtr->Framerate, this->m_ioPtr->Framerate);

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    ImGuiLayer::ImGuiLayer()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        this->m_ioPtr = &ImGui::GetIO();

        this->m_ioPtr->BackendPlatformName = "GJGO-Hangar2";

        this->m_ioPtr->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        this->m_ioPtr->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        this->m_ioPtr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        this->m_ioPtr->KeyMap[ImGuiKey_Tab] = HGR_tab;
        this->m_ioPtr->KeyMap[ImGuiKey_LeftArrow] = HGR_left;
        this->m_ioPtr->KeyMap[ImGuiKey_RightArrow] = HGR_right;
        this->m_ioPtr->KeyMap[ImGuiKey_UpArrow] = HGR_up;
        this->m_ioPtr->KeyMap[ImGuiKey_DownArrow] = HGR_down;
        //io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        //io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        //io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        //io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        //io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        this->m_ioPtr->KeyMap[ImGuiKey_Delete] = HGR_delete;
        this->m_ioPtr->KeyMap[ImGuiKey_Backspace] = HGR_backspace;
        this->m_ioPtr->KeyMap[ImGuiKey_Space] = HGR_space;
        this->m_ioPtr->KeyMap[ImGuiKey_Enter] = HGR_enter;
        this->m_ioPtr->KeyMap[ImGuiKey_Escape] = HGR_escape;
        //io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        this->m_ioPtr->KeyMap[ImGuiKey_A] = HGR_a;
        this->m_ioPtr->KeyMap[ImGuiKey_C] = HGR_c;
        this->m_ioPtr->KeyMap[ImGuiKey_V] = HGR_v;
        this->m_ioPtr->KeyMap[ImGuiKey_X] = HGR_x;
        this->m_ioPtr->KeyMap[ImGuiKey_Y] = HGR_y;
        this->m_ioPtr->KeyMap[ImGuiKey_Z] = HGR_z;

        this->m_ioPtr->SetClipboardTextFn = ImGuiClipboardReadTextCallback;
        this->m_ioPtr->GetClipboardTextFn = ImGuiClipboardGetTextCallback;

        ImGui_ImplOpenGL3_Init("#version 300 es");
    }
}
