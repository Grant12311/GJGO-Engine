#ifndef GJGO_TRANSFORM2D_H
#define GJGO_TRANSFORM2D_H

#include <glm/mat4x4.hpp>

namespace GJGO
{
    struct Position2D
    {
        int x = 0;
        int y = 0;

        Position2D operator+(const Position2D &a_rhs)
        {
            return Position2D{this->x + a_rhs.x, this->y + a_rhs.y};
        }

        Position2D operator-(const Position2D &a_rhs)
        {
            return Position2D{this->x - a_rhs.x, this->y - a_rhs.y};
        }
    };

    inline std::ostream& operator<<(std::ostream &a_os, const Position2D &a_pos)
    {
        return a_os << "(" << a_pos.x << ", " << a_pos.y << ")";
    }

    struct Size2D
    {
        unsigned int width  = 0;
        unsigned int height = 0;
    };

    struct Transform2D
    {
        Position2D position;
        Size2D size;
        glm::mat4 matrix = glm::mat4(1.0f);
    };
}

#endif
