#ifndef GJGO_RENDERER2D_H
#define GJGO_RENDERER2D_H

#include <GJGO/2D/transform2D.hpp>
#include <GJGO/color.hpp>

namespace GJGO
{
    namespace Renderer
    {
        void drawQuad(const Position2D a_position, const Size2D a_size, const Color3 a_color = {1.0f, 1.0f, 1.0f});
    }
}

#endif
