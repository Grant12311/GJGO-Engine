#include <stdexcept>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <GJGO/components.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/log.hpp>
#include <GJGO/texture.hpp>

namespace GJGO
{
    TagComponent::TagComponent(const std::string &a_name) :
        name(a_name) {}

    Transform2DComponent::Transform2DComponent(const glm::vec3 &a_position, const glm::vec2 &a_size, const float a_rotation) :
        position(a_position), size(a_size), rotation(a_rotation) {}

    [[nodiscard]]
    Transform2DComponent Transform2DComponent::getAbsoluteTransform(const Entity a_entity)
    {
        return m_getAbsoluteTransform(a_entity, a_entity.getComponent<Transform2DComponent>());
    }

    [[nodiscard]]
    Transform2DComponent Transform2DComponent::m_getAbsoluteTransform(const Entity a_entity, const Transform2DComponent &a_transform)
    {
        const RelationsComponent& relations = a_entity.getComponent<RelationsComponent>();

        if (relations.parent == Entity())
            return a_transform;

        const Transform2DComponent& parentTransform = relations.parent.hasComponent<Transform2DComponent>() ? relations.parent.getComponent<Transform2DComponent>() : Transform2DComponent();

        return m_getAbsoluteTransform(relations.parent, {a_transform.position + parentTransform.position, a_transform.size/* + parentTransform.size*/, a_transform.rotation + parentTransform.rotation});
    }

    SpriteComponent::SpriteComponent(Texture2D* const a_texture, const glm::vec4 &a_color) :
        texture(a_texture), color(a_color) {}

    Camera2DComponent::Camera2DComponent(const glm::vec2 &a_viewportSize) :
        viewportSize(a_viewportSize) {}

    [[nodiscard]]
    glm::mat4 Camera2DComponent::getOrthoMatrix() const
    {
        return glm::ortho(0.0f, this->viewportSize.x, 0.0f, this->viewportSize.y, -100.0f, 100.0f);
    }

    CollisionBox2DComponent::CollisionBox2DComponent(const glm::vec2 &a_positionOffset, const glm::vec2 &a_sizeMod, const std::function<void(Entity, Entity)> a_whileCollide, const std::function<void(Entity, Entity)> a_onEnter, const std::function<void(Entity, Entity)> a_onExit) :
        positionOffset(a_positionOffset), sizeMod(a_sizeMod), whileCollide(a_whileCollide), onEnter(a_onEnter), onExit(a_onExit) {}
}
