#ifndef GJGO_WINDOW_H
#define GJGO_WINDOW_H

#include <array>

#include <glm/vec2.hpp>

namespace GJGO
{
    namespace Window
    {
        void setTitle(const char* const a_title);

        [[nodiscard]]
        float getPositionX();
        [[nodiscard]]
        float getPositionY();
        [[nodiscard]]
        glm::vec2 getPosition();

        [[nodiscard]]
        float getWidth();
        [[nodiscard]]
        float getHeight();
        [[nodiscard]]
        glm::vec2 getSize();
        void setSize(const int a_width, const int a_height);

        void maximize();
        void restore();
    }
}

#endif
