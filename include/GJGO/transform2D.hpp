#ifndef GJGO_TRANSFORM2D_H
#define GJGO_TRANSFORM2D_H

#include <glm/mat4x4.hpp>

namespace GJGO
{
    struct Position2D
    {
        int x = 0;
        int y = 0;
    };

    struct Size2D
    {
        unsigned int width  = 0;
        unsigned int height = 0;
    };

    struct Transform2D
    {
        Position2D position;
        Size2D size;
        glm::mat4 = glm::mat4(1.0f);
    };
}

#endif
