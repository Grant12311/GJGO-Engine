#include <cassert>
#include <limits>
#include <string>

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
    Entity Entity::getByName(const std::string &a_name)
    {
        auto view = App::instance->registry.view<TagComponent>();

        for (const entt::entity l_entity : view)
        {
            if (view.get<TagComponent>(l_entity).name == a_name)
                return l_entity;
        }

        return Entity();
    }

    [[nodiscard]]
    bool Entity::isValid() const
    {
        return this->m_entity != entt::null;
    }

    [[nodiscard]]
    entt::entity Entity::getRaw() const
    {
        return this->m_entity;
    }
}
