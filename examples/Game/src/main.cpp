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
    void onUpdate() override {}

    void onEvent(GJGO::Event* const a_event) override
    {
        switch (a_event->type)
        {
            case GJGO::EventType::keyDown:
                GJGO_LOG_INFO("Key Down: ", a_event->keycode);
                if (a_event->keycode == HGR_0){
                    GJGO::g_appInstancePtr->window.setVsync(0);
                }else if (a_event->keycode == HGR_1){
                    GJGO::g_appInstancePtr->window.setVsync(1);
                    GJGO::g_appInstancePtr->window.setFramerateCap(60);
                }else if (a_event->keycode == HGR_2){
                    GJGO::g_appInstancePtr->window.setVsync(1);
                    GJGO::g_appInstancePtr->window.setFramerateCap(30);
                }

                if (a_event->keycode == HGR_up){
                    GJGO::g_appInstancePtr->window.setSize(100, 100);
                }else if (a_event->keycode == HGR_down){
                    GJGO::g_appInstancePtr->window.setSize(1000, 1000);
                }
                break;
            case GJGO::EventType::keyUp:
                GJGO_LOG_INFO("Key Up: ", a_event->keycode);
                break;
            case GJGO::EventType::mouseMove:
                GJGO_LOG_INFO("(", a_event->mousePosition.absolute.x, ", ", a_event->mousePosition.absolute.y, ")");
                break;
        }
    }

    void draw() override
    {
        //GJGO_LOG_INFO("Draw!");
    }
};

int main()
{
    GJGO_LOG_SET_PRINT_FILE(false);
    GJGO_LOG_SET_PRINT_FUNCTION(false);
    Hangar::Config winConfig;
    winConfig.vsync = false;
    GJGO::Application app(winConfig);

    app.layers.emplace_back(new GameLayer);

    GJGO::Entity e(&app);
    e.addComponent<GJGO::Position2D>(5, 5);
    e.editComponent<GJGO::Position2D>(0, 90);

    GJGO_LOG_INFO("(", e.getComponent<GJGO::Position2D>().x, ", ", e.getComponent<GJGO::Position2D>().y, ")");

    app.run();

    return 0;
}
