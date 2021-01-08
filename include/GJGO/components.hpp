#ifndef GJGO_COMPONENTS_H
#define GJGO_COMPONENTS_H

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <Druid/texture.h>

namespace GJGO
{
    struct TagComponent
    {
        std::string tag;
    };

    struct Transform2DComponent
    {
        glm::vec2 position{0, 0};
        glm::vec2 size{0, 0};
    };

    struct SpriteComponent
    {
        Druid::Texture2D* texture;
        glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
    };
}

#endif
