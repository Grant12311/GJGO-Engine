#ifndef GJGO_LAYER_H
#define GJGO_LAYER_H

#include <GJGO/app.hpp>
#include <GJGO/event.hpp>

namespace GJGO
{
    class Layer
    {
    public:
        GJGO::Application* parentPtr;

        virtual void onUpdate() {}
        virtual void onEvent(Event* const a_event)  {}
        virtual void draw() {}
    };
}

#endif
