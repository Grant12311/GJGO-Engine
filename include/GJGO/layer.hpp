#ifndef GJGO_LAYER_H
#define GJGO_LAYER_H

#include <GJGO/event.hpp>

namespace GJGO
{
    class Application;

    class Layer
    {
    public:
        virtual void onUpdate() {}
        virtual void onEvent(Event* const /*a_event*/)  {}
        virtual void draw() {}

        virtual ~Layer() = default;
    protected:
        Application* parentPtr;

        Layer(Application* const a_appPtr);
    };
}

#endif
