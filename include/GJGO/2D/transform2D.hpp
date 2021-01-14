#ifndef GJGO_TRANSFORM2D_H
#define GJGO_TRANSFORM2D_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace GJGO
{
    struct Position2D
    {
        int x;
        int y;

        Position2D operator+(const Position2D &a_rhs)
        {
            return Position2D{this->x + a_rhs.x, this->y + a_rhs.y};
        }

        Position2D operator-(const Position2D &a_rhs)
        {
            return Position2D{this->x - a_rhs.x, this->y - a_rhs.y};
        }

        Position2D(const int a_x = 0, const int a_y = 0) :
            x(a_x), y(a_y) {}

        Position2D(const glm::vec2 &a_vec = {0, 0}) :
            x(static_cast<int>(a_vec.x)), y(static_cast<int>(a_vec.y)) {}
    };

    inline std::ostream& operator<<(std::ostream &a_os, const Position2D &a_pos)
    {
        return a_os << "(" << a_pos.x << ", " << a_pos.y << ")";
    }

    struct Size2D
    {
        unsigned int width;
        unsigned int height;

        Size2D(const unsigned int a_width = 0, const unsigned int a_height = 0) :
            width(a_width), height(a_height) {}

        Size2D(const glm::vec2 &a_vec = {0, 0}) :
            width(static_cast<unsigned int>(a_vec.x)), height(static_cast<unsigned int>(a_vec.y)) {}
    };

    struct Transform2D
    {
        Position2D position;
        Size2D size;
        glm::mat4 matrix = glm::mat4(1.0f);
    };
}

#endif
