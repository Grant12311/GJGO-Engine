#ifndef GJGO_RENDERER2D_H
#define GJGO_RENDERER2D_H

#include <Druid/shader.h>

#include <GJGO/2D/transform2D.hpp>
#include <GJGO/color.hpp>

namespace GJGO
{
    namespace Renderer
    {
        void begin2D(Druid::Shader* const a_shader, const unsigned int a_width, const unsigned int a_height);

        void drawQuad(Druid::Shader* const a_shader, const Position2D &a_position, const Size2D &a_size, const Color3 a_color = {1.0f, 1.0f, 1.0f});

        void genOrthoMatrix(const unsigned int a_width, const unsigned int a_height);
        void init2D();
    }
}

#endif
