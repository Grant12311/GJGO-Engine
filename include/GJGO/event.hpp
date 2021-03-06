#ifndef GJGO_EVENT_H
#define GJGO_EVENT_H

#include <cstdint>
#include <array>

#include <GJGO/2D/transform2D.hpp>

namespace GJGO
{
    enum class EventType
    {
        keyDown,
        keyUp,
        keyTypedDown,
        keyTypedUp,
        mouseMove,
        mouseButtonDown,
        mouseButtonUp,
        mouseWheelScroll,
        windowResize
    };

    struct MousePositionEventData
    {
        Position2D relative;
        Position2D absolute;
    };

    struct Event
    {
        const EventType type;
        bool handled = false;
        union
        {
            int keycode;

            MousePositionEventData mousePosition;
            uint8_t mouseButton;
            signed char mouseWheelDirection;

            Size2D windowSize;
        };

        Event(const EventType a_type) :
            type(a_type) {}
    };
}

#endif
