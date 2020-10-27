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
        union
        {
            const int32_t keycode;
            const int8_t mouseButton;
        };
    };
}

#endif
