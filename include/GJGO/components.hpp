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
        std::string name;

        TagComponent(const std::string &a_name = "") :
            name(a_name) {}
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

    struct CollisionBox2DComponent
    {
        std::function<void(Entity, Entity)> whileCollide, onEnter, onExit;

        CollisionBox2DComponent(const std::function<void(Entity, Entity)> a_whileCollide = {}, const std::function<void(Entity, Entity)> a_onEnter = {}, const std::function<void(Entity, Entity)> a_onExit = {}) :
            whileCollide(a_whileCollide), onEnter(a_onEnter), onExit(a_onExit) {}
    };

    [[nodiscard]]
    inline constexpr bool rigidBodiesCollied(const Transform2DComponent &a_x, const Transform2DComponent &a_y)
    {
        return a_x.position.x < a_y.position.x + a_y.size.x && a_x.position.x + a_x.size.x > a_y.position.x && a_x.position.y < a_y.position.y + a_y.size.y && a_x.position.y + a_x.size.y > a_y.position.y;
    }
}

#endif
