#ifndef GJGO_COMPONENTS_H
#define GJGO_COMPONENTS_H

#include <functional>
#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <GJGO/entity.hpp>
#include <GJGO/texture.hpp>

namespace GJGO
{
    struct TagComponent
    {
        unsigned long long int uuid;
        std::string name;

        TagComponent(const unsigned long long int a_uuid = 0, const std::string &a_name = "") :
            uuid(a_uuid), name(a_name) {}
    };

    struct Transform2DComponent
    {
        glm::vec3 position;
        glm::vec2 size;
        float rotation;

        constexpr Transform2DComponent(const glm::vec3 &a_position = glm::vec3(1.0f), const glm::vec2 &a_size = glm::vec2(1.0f), const float a_rotation = 0.0f) :
            position(a_position), size(a_size), rotation(a_rotation) {}
    };

    struct SpriteComponent
    {
        Texture2D* texture;
        glm::vec4 color;

        constexpr SpriteComponent(Texture2D* const a_texture = nullptr, const glm::vec4 &a_color = glm::vec4(1.0f)) :
            texture(a_texture), color(a_color) {}
    };

    struct RigidBody2DComponent
    {
        std::function<void(Entity, Entity)> action;

        RigidBody2DComponent(const std::function<void(Entity, Entity)> a_action = {}) :
            action(a_action) {}
    };

    [[nodiscard]]
    inline constexpr bool rigidBodiesCollied(const Transform2DComponent &a_x, const Transform2DComponent &a_y)
    {
        return a_x.position.x < a_y.position.x + a_y.size.x && a_x.position.x + a_x.size.x > a_y.position.x && a_x.position.y < a_y.position.y + a_y.size.y && a_x.position.y + a_x.size.y > a_y.position.y;
    }
}

#endif
