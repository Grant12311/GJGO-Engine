#include <iostream>

#include <GJGO/app.hpp>
#include <GJGO/log.hpp>
#include <GJGO/entity.hpp>

void onUpdate(GJGO::Application* const a_appPtr)
{
    a_appPtr->window.setTitle(a_appPtr->window.deltaTime);
}

struct Position2D
{
    int x = 0;
    int y = 0;
};

int main()
{
    GJGO::Log::printFile = false;
    GJGO::Log::printFunction = false;
    GJGO::Application app;

    GJGO::Entity e(&app);
    e.addComponent<Position2D>(5, 5);
    e.editComponent<Position2D>(0, 90);

    GJGO_LOG_INFO("(", e.getComponent<Position2D>().x, ", ", e.getComponent<Position2D>().y, ")");

    app.onUpdateEvent.addListener(onUpdate);
    app.run();

    return 0;
}
