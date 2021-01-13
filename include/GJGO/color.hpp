#ifndef GJGO_COLOR3_H
#define GJGO_COLOR3_H

#include <glm/vec4.hpp>

namespace GJGO
{
    struct Color3
    {
        float red = 1.0f;
        float green = 1.0f;
        float blue = 1.0f;
    };

    struct Color4
    {
        float red = 1.0f;
        float green = 1.0f;
        float blue = 1.0f;
        float alpha = 1.0f;

        Color4(const float a_red = 0.0f, const float a_green = 0.0f, const float a_blue = 0.0f, const float a_alpha = 0.0f) :
            red(a_red), green(a_green), blue(a_blue), alpha(a_alpha) {}

        Color4(const glm::vec4 &a_vec = {0.0f, 0.0f, 0.0f, 0.0f}) :
            red(a_vec.x), green(a_vec.y), blue(a_vec.z), alpha(a_vec.w) {}
    };
}

#endif
