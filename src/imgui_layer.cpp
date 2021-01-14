#include <GJGOpch.hpp>

#include <GJGO/app.hpp>
#include <GJGO/clipboard.hpp>
#include <GJGO/display.hpp>
#include <GJGO/imgui_layer.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    static void ImGuiClipboardReadTextCallback(void* const /*a_userData*/, const char* const a_text)
    {
        GJGO_PROFILE_FUNCTION();

        Clipboard::writeText(a_text);
    }

    static const char* ImGuiClipboardGetTextCallback(void* const /*a_userData*/)
    {
        GJGO_PROFILE_FUNCTION();

        return Clipboard::readText();
    }

    void ImGuiLayer::onUpdate()
    {
        GJGO_PROFILE_FUNCTION();

        this->m_ioPtr->KeyCtrl = glfwGetKey(Application::instance->windowPtr, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(Application::instance->windowPtr, GLFW_KEY_RIGHT_CONTROL);
        this->m_ioPtr->KeyShift = glfwGetKey(Application::instance->windowPtr, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(Application::instance->windowPtr, GLFW_KEY_RIGHT_SHIFT);
        this->m_ioPtr->KeyAlt = glfwGetKey(Application::instance->windowPtr, GLFW_KEY_LEFT_ALT) || glfwGetKey(Application::instance->windowPtr, GLFW_KEY_RIGHT_ALT);
        this->m_ioPtr->KeySuper = glfwGetKey(Application::instance->windowPtr, GLFW_KEY_LEFT_SUPER) || glfwGetKey(Application::instance->windowPtr, GLFW_KEY_RIGHT_SUPER);

        this->m_ioPtr->DisplaySize = ImVec2(static_cast<float>(Window::getWidth()), static_cast<float>(Window::getHeight()));
        this->m_ioPtr->DeltaTime = static_cast<float>(Window::deltaTime) / 1000.0f;
    }

    void ImGuiLayer::onEvent(Event* const a_eventPtr)
    {
        GJGO_PROFILE_FUNCTION();

        switch (a_eventPtr->type)
        {
            case EventType::keyDown:
                this->m_ioPtr->KeysDown[a_eventPtr->keycode] = true;
                a_eventPtr->handled = this->m_ioPtr->WantCaptureKeyboard;
                break;
            case EventType::keyUp:
                this->m_ioPtr->KeysDown[a_eventPtr->keycode] = false;
                a_eventPtr->handled = this->m_ioPtr->WantCaptureKeyboard;
                break;
            case EventType::keyTypedDown:
                this->m_ioPtr->AddInputCharacter(static_cast<unsigned short>(a_eventPtr->keycode));
                this->m_ioPtr->KeysDown[a_eventPtr->keycode] = true;
                a_eventPtr->handled = this->m_ioPtr->WantCaptureKeyboard;
                break;
            /*case EventType::keyTypedUp:
                this->m_ioPtr->KeysDown[a_eventPtr->keycode] = false;
                a_eventPtr->handled = this->m_ioPtr->WantCaptureKeyboard;
                break;*/
            case EventType::mouseMove:
                this->m_ioPtr->MousePos = ImVec2(static_cast<float>(a_eventPtr->mousePosition.relative.x), static_cast<float>(Window::getHeight() - a_eventPtr->mousePosition.relative.y));
                break;
            case EventType::mouseButtonDown:
                this->m_ioPtr->MouseDown[a_eventPtr->mouseButton] = true;
                //a_eventPtr->handled = this->m_ioPtr->WantCaptureMouse;
                break;
            case EventType::mouseButtonUp:
                this->m_ioPtr->MouseDown[a_eventPtr->mouseButton] = false;
                //a_eventPtr->handled = this->m_ioPtr->WantCaptureMouse;
                break;
            case EventType::mouseWheelScroll:
                this->m_ioPtr->MouseWheel += static_cast<float>(a_eventPtr->mouseWheelDirection);
                break;
            case EventType::windowResize:
                this->m_ioPtr->DisplaySize = ImVec2(static_cast<float>(Window::getWidth()), static_cast<float>(Window::getHeight()));
                break;
        }
    }

    ImGuiLayer::ImGuiLayer()
    {
        GJGO_PROFILE_FUNCTION();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        this->m_ioPtr = &ImGui::GetIO();

        this->m_ioPtr->BackendPlatformName = "GJGO-Hangar2";

        this->m_ioPtr->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        this->m_ioPtr->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        this->m_ioPtr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        this->m_ioPtr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        this->m_ioPtr->KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        this->m_ioPtr->KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        this->m_ioPtr->KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        this->m_ioPtr->KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        this->m_ioPtr->KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        this->m_ioPtr->KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        this->m_ioPtr->KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        this->m_ioPtr->KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        this->m_ioPtr->KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        this->m_ioPtr->KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        this->m_ioPtr->KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        this->m_ioPtr->KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        this->m_ioPtr->KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        this->m_ioPtr->KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        this->m_ioPtr->KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        this->m_ioPtr->KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        this->m_ioPtr->KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        this->m_ioPtr->KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        this->m_ioPtr->KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        this->m_ioPtr->KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        this->m_ioPtr->KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        this->m_ioPtr->KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        this->m_ioPtr->SetClipboardTextFn = ImGuiClipboardReadTextCallback;
        this->m_ioPtr->GetClipboardTextFn = ImGuiClipboardGetTextCallback;

        ImGui_ImplOpenGL3_Init("#version 300 es");
    }

    ImGuiLayer::~ImGuiLayer()
    {
        GJGO_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
    }
}
