#include <cassert>
#include <limits>
#include <string>

#include <entt/entity/registry.hpp>

#include <GJGO/components.hpp>
#include <GJGO/entity.hpp>

namespace GJGO
{
    static unsigned long long int uuidCounter = 0;

    Entity::Entity(const entt::entity a_entity) :
        isValid(a_entity == entt::null ? false : true), m_entity(a_entity) {}

    Entity Entity::create(const std::string &a_name)
    {
        assert(uuidCounter != std::numeric_limits<unsigned long long int>::max());

        Entity created = App::instance->registry.create();
        created.addComponent<TagComponent>(uuidCounter++, a_name);
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
}
