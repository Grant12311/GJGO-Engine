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
                //GJGO_LOG_INFO("(", a_event->mousePosition.absolute[0], ", ", a_event->mousePosition.absolute[1], ")");
                break;
            case GJGO::EventType::windowResize:
                GJGO_LOG_INFO("(", a_event->windowSize[0], ", ", a_event->windowSize[1], ")");
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

int main()
{
    GJGO_LOG_SET_PRINT_FILE(false);
    GJGO_LOG_SET_PRINT_FUNCTION(false);
    GJGO::Application app;

    app.layers.emplace_back(new GameLayer(&app));

    GJGO::Entity e(&app);
    e.addComponent<Position2D>(5, 5);
    e.editComponent<Position2D>(0, 90);

    GJGO_LOG_INFO("(", e.getComponent<Position2D>().x, ", ", e.getComponent<Position2D>().y, ")");

    app.run();

    return 0;
}
