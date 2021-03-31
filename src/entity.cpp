#include <string>

#include <entt/entity/registry.hpp>

#include <GJGO/components.hpp>
#include <GJGO/entity.hpp>

namespace GJGO
{
    Entity::Entity(const entt::entity a_entity) :
        isValid(a_entity == entt::null ? false : true), m_entity(a_entity) {}

    Entity Entity::create(const std::string &a_name)
    {
        Entity created = App::instance->registry.create();
        created.addComponent<TagComponent>(a_name);
        return created;
    }

    Entity Entity::create()
    {
        return App::instance->registry.create();
    }

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
}
