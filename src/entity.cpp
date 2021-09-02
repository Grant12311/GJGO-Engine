#include <algorithm>
#include <string>
#include <optional>

#include <entt/entity/registry.hpp>

#include <GJGO/components.hpp>
#include <GJGO/entity.hpp>

namespace GJGO
{
    Entity::Entity(const entt::entity a_entity) :
        m_entity(a_entity) {}

    Entity Entity::create()
    {
        return App::instance->registry.create();
    }

    Entity Entity::create(const std::string &a_name)
    {
        Entity created = App::instance->registry.create();
        created.addComponent<TagComponent>(a_name);
        return created;
    }

    [[nodiscard]]
    std::optional<Entity> Entity::getByName(const std::string &a_name)
    {
        auto view = App::instance->registry.view<TagComponent>();

        auto it = std::find_if(view.begin(), view.end(), [&view, &a_name](const entt::entity a_entity) -> bool
        {
            return view.get<TagComponent>(a_entity).name == a_name;
        });

        return it == view.end() ? std::optional<Entity>() : *it;
    }

    [[nodiscard]]
    bool Entity::isValid() const
    {
        return App::instance->registry.valid(this->m_entity);
    }

    [[nodiscard]]
    entt::entity Entity::getRaw() const
    {
        return this->m_entity;
    }
}
