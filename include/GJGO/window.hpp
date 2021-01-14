#ifndef GJGO_WINDOW_H
#define GJGO_WINDOW_H

#include <array>

namespace GJGO
{
    namespace Window
    {
        inline double deltaTime = 0;

        void setTitle(const char* const a_title);

        std::array<int, 2> getPosition();

        unsigned int getWidth();
        unsigned int getHeight();
        void setSize(const int a_width, const int a_height);

        void maximize();
        void restore();
    }
}

#endif
