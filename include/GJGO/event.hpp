#ifndef GJGO_EVENT_H
#define GJGO_EVENT_H

#include <cstdint>
#include <array>

#include <GJGO/position2D.hpp>
#include <GJGO/size2D.hpp>

namespace GJGO
{
    enum class EventType
    {
        keyDown,
        keyUp,
        keyTyped,
        mouseMove,
        mouseButtonDown,
        mouseButtonUp,
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
            uint16_t keycode;

            MousePositionEventData mousePosition;
            uint8_t mouseButton;

            Size2D windowSize;
        };

        Event(const EventType a_type) :
            type(a_type) {}
    };
}

#endif
