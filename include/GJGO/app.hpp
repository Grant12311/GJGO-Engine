#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <vector>

#include <entt/entity/registry.hpp>

#include <Hangar2/hangar.h>

#include <Beacon/beacon.h>

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
    };
}

#endif
