#ifndef GJGO_COMPONENTS_H
#define GJGO_COMPONENTS_H

#include <functional>
#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <GJGO/entity.hpp>

namespace GJGO
{
    struct TagComponent
    {
        std::string name;

        TagComponent(const std::string &a_name = "");
    };

    struct RelationsComponent
    {
        Entity parent;
        std::vector<Entity> children;
    };

    class Transform2DComponent
    {
    public:
        glm::vec3 position;
        glm::vec2 size;
        float rotation;

        Transform2DComponent(const glm::vec3 &a_position = glm::vec3(1.0f), const glm::vec2 &a_size = glm::vec2(1.0f), const float a_rotation = 0.0f);

        [[nodiscard]]
        static Transform2DComponent getAbsoluteTransform(const Entity a_entity);
    private:
        [[nodiscard]]
        static Transform2DComponent m_getAbsoluteTransform(const Entity a_entity, const Transform2DComponent &a_transform);
    };

    struct SpriteComponent
    {
        Texture2D* texture;
        glm::vec4 color;

        SpriteComponent(Texture2D* const a_texture = nullptr, const glm::vec4 &a_color = glm::vec4(1.0f));
    };

    struct Camera2DComponent
    {
        glm::vec2 viewportSize;

        Camera2DComponent(const glm::vec2 &a_viewportSize = glm::vec2(0.0));

        [[nodiscard]]
        glm::mat4 getOrthoMatrix() const;
    };

    struct CollisionBox2DComponent
    {
        glm::vec2 positionOffset, sizeMod;
        std::function<void(Entity, Entity)> whileCollide, onEnter, onExit;

        CollisionBox2DComponent(const glm::vec2 &a_positionOffset = glm::vec2(0.0, 0.0), const glm::vec2 &a_sizeMod = glm::vec2(1.0, 1.0), const std::function<void(Entity, Entity)> a_whileCollide = {}, const std::function<void(Entity, Entity)> a_onEnter = {}, const std::function<void(Entity, Entity)> a_onExit = {});
    };

    [[nodiscard]]
    inline constexpr bool rigidBodiesCollied(const glm::vec2 &a_pos1, const glm::vec2 &a_pos2, const glm::vec2 &a_size1, const glm::vec2 &a_size2)
    {
        return a_pos1.x < a_pos2.x + a_size2.x && a_pos1.x + a_size1.x > a_pos2.x && a_pos1.y < a_pos2.y + a_size2.y && a_pos1.y + a_size1.y > a_pos2.y;
    }
}

#endif
