#include <iostream>

#include <Hangar2/keycodes.h>

#include <GJGO/app.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/event.hpp>
#include <GJGO/imgui_layer.hpp>
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

int main()
{
    GJGO_LOG_SET_PRINT_FILE(false);
    GJGO_LOG_SET_PRINT_FUNCTION(false);
    GJGO::Application app;

    app.layers.emplace_back(new GameLayer);
    app.layers.emplace_back(new GJGO::ImGuiLayer);

    GJGO::Entity e(&app);
    e.addComponent<GJGO::Position2D>(5, 5);
    e.editComponent<GJGO::Position2D>(0, 90);

    GJGO_LOG_INFO("(", e.getComponent<GJGO::Position2D>().x, ", ", e.getComponent<GJGO::Position2D>().y, ")");

    app.run();

    return 0;
}
