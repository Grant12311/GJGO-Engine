#ifndef GJGO_APP_H
#define GJGO_APP_H

#include <Hangar2/hangar.h>

#include <Beacon/beacon.h>

namespace GJGO
{
    class Application
    {
    public:
        Hangar::Window window;

        Beacon::Event<GJGO::Application*> onUpdateEvent;

        void run();

        Application(const Hangar::Config &a_config = Hangar::Config());
    };
}

#endif
