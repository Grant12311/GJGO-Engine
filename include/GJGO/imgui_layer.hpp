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

        std::array<int, 6> noDrawKeycodes = {
            HGR_delete, HGR_enter, HGR_right, HGR_left, HGR_up, HGR_down
        };
    public:
        virtual void onUpdate() override;
        virtual void onEvent(Event* const a_eventPtr) override;

        ImGuiLayer();
    };
}

#endif
