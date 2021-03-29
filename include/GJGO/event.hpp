#ifndef GJGO_EVENT_H
#define GJGO_EVENT_H

#include <array>

#include <glm/vec2.hpp>

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
        glm::vec2 relative;
        glm::vec2 absolute;
    };

    struct Event
    {
        const EventType type;
        bool handled = false;
        union
        {
            int keycode;

            MousePositionEventData mousePosition;
            unsigned char mouseButton;
            signed char mouseWheelDirection;

            glm::vec2 windowSize;
        };

        Event(const EventType a_type) :
            type(a_type) {}
    };
}

#endif
