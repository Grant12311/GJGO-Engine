#ifndef GJGO_WINDOW_H
#define GJGO_WINDOW_H

#include <array>

#include <GJGO/2D/transform2D.hpp>

namespace GJGO
{
    namespace Window
    {
        inline double deltaTime = 0;

        void setTitle(const char* const a_title);

        int getPositionX();
        int getPositionY();
        Position2D getPosition();

        unsigned int getWidth();
        unsigned int getHeight();
        Size2D getSize();
        void setSize(const int a_width, const int a_height);

        void maximize();
        void restore();
    }
}

#endif
