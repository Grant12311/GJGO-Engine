#ifndef GJGO_EVENT_H
#define GJGO_EVENT_H

#include <cstdint>
#include <array>

namespace GJGO
{
    enum class EventType
    {
        keyDown,
        keyUp,
        mouseMove
    };

    struct MousePositionEventData
    {
        std::array<int, 2> relative;
        std::array<int, 2> absolute;
    };

    struct Event
    {
        const EventType type;
        bool handled = false;
        union
        {
            int32_t keycode;
            MousePositionEventData mousePosition;
        };

        Event(const EventType a_type) :
            type(a_type) {}
    };
}

#endif
