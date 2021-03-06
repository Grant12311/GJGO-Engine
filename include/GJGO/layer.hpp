#ifndef GJGO_LAYER_H
#define GJGO_LAYER_H

#include <GJGO/event.hpp>

namespace GJGO
{
    class App;

    class Layer
    {
    public:
        const char* name;

        virtual void onUpdate() {}
        virtual void onEvent(Event* const /*a_event*/)  {}
        virtual void draw() {}
        virtual void drawGui() {}

        virtual ~Layer() = default;
    };
}

#endif
