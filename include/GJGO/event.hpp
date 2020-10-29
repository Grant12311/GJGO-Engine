#ifndef GJGO_EVENT_H
#define GJGO_EVENT_H

#include <cstdint>
#include <array>

#include <GJGO/position2D.hpp>

namespace GJGO
{
    enum class EventType
    {
        keyDown,
        keyUp,
        mouseMove,
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
            int32_t keycode;

            MousePositionEventData mousePosition;

            std::array<unsigned int, 2> windowSize;
        };

        Event(const EventType a_type) :
            type(a_type) {}
    };
}

#endif
