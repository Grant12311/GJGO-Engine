#ifndef GJGO_WINDOW_H
#define GJGO_WINDOW_H

#include <array>

#include <glm/vec2.hpp>

namespace GJGO
{
    namespace Window
    {
        inline double deltaTime = 0;

        void setTitle(const char* const a_title);

        int getPositionX();
        int getPositionY();
        glm::vec2 getPosition();

        int getWidth();
        int getHeight();
        glm::vec2 getSize();
        void setSize(const int a_width, const int a_height);

        void maximize();
        void restore();
    }
}

#endif
