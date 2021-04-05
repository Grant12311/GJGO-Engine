#ifndef GJGO_RENDERER2D_H
#define GJGO_RENDERER2D_H

#include <Druid/shader.h>

#include <GJGO/camera2D.hpp>
#include <GJGO/texture.hpp>

namespace GJGO
{
    namespace Renderer
    {
        void begin2D(const Camera2D &a_camera, const unsigned int a_width, const unsigned int a_height);

        void drawQuad(const glm::vec2 &a_position, const glm::vec2 &a_size, const float a_rotation = 0.0f, const glm::vec4 &a_color = {1.0f, 1.0f, 1.0f, 1.0f}, GJGO::Texture* const a_texture = nullptr);

        void init2D();
        void shutdown2D();
    }
}

#endif
