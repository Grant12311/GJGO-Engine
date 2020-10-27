#include <GJGO/event.hpp>

namespace GJGO
{
    Event::Event(const EventType a_type, const EventData &a_data) :
        type(a_type), data(a_data) {}
}
