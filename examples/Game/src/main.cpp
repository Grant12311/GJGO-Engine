#include <iostream>

#include <GJGO/app.hpp>

void onUpdate(GJGO::Application* const a_app)
{
    a_app->window.setTitle(a_app->window.deltaTime);
}

int main()
{
    GJGO::Application app;
    app.onUpdateEvent.addListener(onUpdate);
    app.run();

    return 0;
}
