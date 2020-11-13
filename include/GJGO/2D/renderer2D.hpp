#ifndef GJGO_RENDERER2D_H
#define GJGO_RENDERER2D_H

#include <Druid/shader.h>

#include <GJGO/color.hpp>
#include <GJGO/2D/camera2D.hpp>
#include <GJGO/2D/transform2D.hpp>

namespace GJGO
{
    namespace Renderer
    {
        void begin2D(Druid::Shader* const a_shader, const Camera2D &a_camera, const unsigned int a_width, const unsigned int a_height);

        void drawQuad(const Position2D &a_position, const Size2D &a_size, const float a_rotation = 0.0f, const Color3 a_color = {1.0f, 1.0f, 1.0f}, const unsigned int a_texID = 0);

        void genOrthoMatrix(const unsigned int a_width, const unsigned int a_height);
        void init2D();
    }
}

#endif
