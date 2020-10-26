#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <entt/entity/registry.hpp>

#include <Hangar2/hangar.h>

#include <Beacon/beacon.h>

namespace GJGO
{
    class Application
    {
    public:
        Hangar::Window window;
        entt::registry registry;

        Beacon::Event<GJGO::Application*> onUpdateEvent;

        void run();

        Application(const Hangar::Config &a_config = Hangar::Config());
    };
}

#endif
