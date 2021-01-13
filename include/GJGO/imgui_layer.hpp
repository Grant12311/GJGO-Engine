#ifndef GJGO_IMGUI_LAYER_H
#define GJGO_IMGUI_LAYER_H

#include <array>
#include <algorithm>

#include <imgui.h>

#include <GJGO/layer.hpp>

namespace GJGO
{
    class ImGuiLayer : public Layer
    {
    private:
        ImGuiIO* m_ioPtr;
    public:
        virtual void onUpdate() override;
        virtual void onEvent(Event* const a_eventPtr) override;

        ImGuiLayer();
        ~ImGuiLayer();
    };
}

#endif
