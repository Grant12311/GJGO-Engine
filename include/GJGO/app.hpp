#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <vector>

#include <entt/entity/registry.hpp>

#include <Hangar2/hangar.h>

#include <GJGO/layer.hpp>
#include <GJGO/event.hpp>

namespace GJGO
{
    class Application
    {
    public:
        Hangar::Window window;
        entt::registry registry;

        std::vector<Layer*> layers;
        std::vector<Event*> pendingEvents;

        void run();

        Application(const Hangar::Config &a_config = Hangar::Config());
        ~Application();
    private:
        void hangarOnKeyDownCallback(const int a_keycode);
        void hangarOnKeyUpCallback(const int a_keycode);

        void hangarOnMouseMoveCallback(const int a_posX, const int a_posY, const int a_posXAbs, const int a_posYAbs);

        void hangarOnWindowResizeCallback(const unsigned int a_width, const unsigned int a_height);
    };
}

#endif
