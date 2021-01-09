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
        glm::vec2 position;
        glm::vec2 size;

        Transform2DComponent(const glm::vec2 a_position = {0.0f, 0.0f}, const glm::vec2 a_size = {0.0f, 0.0f}):
            position(a_position), size(a_size) {}
    };

    struct SpriteComponent
    {
        unsigned int texture;
        glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

        SpriteComponent(const unsigned int a_texture = 0, const glm::vec4 a_color = {1.0f, 1.0f, 1.0f, 1.0f}):
            texture(a_texture), color(a_color) {}
    };
}

#endif
