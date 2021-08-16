#ifndef GJGO_MOUSE_H
#define GJGO_MOUSE_H

#include <glm/vec2.hpp>

namespace GJGO
{
    namespace Mouse
    {
        [[nodiscard]]
        glm::vec2 getPosition();
        [[nodiscard]]
        float getPositionX();
        [[nodiscard]]
        float getPositionY();
    }
}

#endif
