#ifndef GJGO_MOUSE_H
#define GJGO_MOUSE_H

#include <GJGO/2D/transform2D.hpp>

namespace GJGO
{
    namespace Mouse
    {
        void setPositionRelative(const int a_x, const int a_y);
        void setPositionAbsolute(const int a_x, const int a_y);
        Position2D getPositionRelative();
        Position2D getPositionAbsolute();
    }
}

#endif
