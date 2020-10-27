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

    union EventData
    {
        const int32_t keycode;
        const int8_t mouseButton;
    };

    struct Event
    {
        Event(const EventType a_type, const EventData &a_data);

        const EventType type;
        const EventData data;
    };
}

#endif
