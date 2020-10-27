#include <iostream>

#include <GJGO/app.hpp>
#include <GJGO/log.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/event.hpp>

struct Position2D
{
    int x = 0;
    int y = 0;
};

class GameLayer : public GJGO::Layer
{
public:
    void onUpdate() override
    {
        GJGO_LOG_INFO("Game Layer Updated!");
    }

    void onEvent(GJGO::Event* const a_event) override
    {
        switch (a_event->type)
        {
            case GJGO::EventType::keyDown:
                GJGO_LOG_INFO("Key Down: ", a_event->data.keycode);
                break;
            case GJGO::EventType::keyUp:
                break;
        }
    }
};

int main()
{
    GJGO::Log::printFile = false;
    GJGO::Log::printFunction = false;
    GJGO::Application app;

    app.layers.emplace_back(new GameLayer);

    GJGO::Entity e(&app);
    e.addComponent<Position2D>(5, 5);
    e.editComponent<Position2D>(0, 90);

    GJGO_LOG_INFO("(", e.getComponent<Position2D>().x, ", ", e.getComponent<Position2D>().y, ")");

    app.run();

    return 0;
}
