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

        Layer(const char* const a_name = "") :
            name(a_name) {}
        virtual ~Layer() = default;

        virtual void onUpdate() {}
        virtual void onEvent(const Event &/*a_event*/)  {}
        virtual void draw() {}
        virtual void drawGui() {}
    };
}

#endif
