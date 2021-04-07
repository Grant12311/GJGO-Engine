#ifndef GJGO_COMPONENTS_H
#define GJGO_COMPONENTS_H

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <GJGO/texture.hpp>

namespace GJGO
{
    struct TagComponent
    {
        std::string name;

        TagComponent(const std::string &a_name = "") :
            name(a_name) {}
    };

    struct Transform2DComponent
    {
        glm::vec2 position;
        glm::vec2 size;
        float rotation;

        Transform2DComponent(const glm::vec2 &a_position = {0.0f, 0.0f}, const glm::vec2 &a_size = {1.0f, 1.0f}, const float a_rotation = 0.0f) :
            position(a_position), size(a_size), rotation(a_rotation) {}
    };

    struct SpriteComponent
    {
        Texture2D* texture;
        glm::vec4 color;
        int layer;

        SpriteComponent(Texture2D* const a_texture = nullptr, const glm::vec4 &a_color = {1.0f, 1.0f, 1.0f, 1.0f}, const int a_layer = 0) :
            texture(a_texture), color(a_color), layer(a_layer) {}
    };
}

#endif
