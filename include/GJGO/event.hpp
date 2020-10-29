#ifndef GJGO_EVENT_H
#define GJGO_EVENT_H

#include <cstdint>

namespace GJGO
{
    enum class EventType
    {
        keyDown = 0,
        keyUp
    };

    struct Event
    {
        const EventType type;
        bool handled = false;
        union
        {
            int32_t keycode;
            std::array<int32_t, 2> mousePosition;
        };

        Event(const EventType a_type) :
            type(a_type) {}
    };
}

#endif
