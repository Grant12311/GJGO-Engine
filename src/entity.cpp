#include <algorithm>
#include <cassert>
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
        Entity result = App::instance->registry.create();
        result.addComponent<RelationsComponent>();

        return result;
    }

    Entity Entity::create(const std::string &a_name)
    {
        Entity result = Entity::create();
        result.addComponent<TagComponent>(a_name);

        return result;
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

    [[nodiscard]]
    Entity Entity::parent() const
    {
        return this->getComponent<RelationsComponent>().parent;
    }

    [[nodiscard]]
    const std::vector<Entity>& Entity::children() const
    {
        return this->getComponent<RelationsComponent>().children;
    }

    void Entity::setParent(Entity a_entity)
    {
        RelationsComponent& relations = this->getComponent<RelationsComponent>();
        RelationsComponent& relationsParent = a_entity.getComponent<RelationsComponent>();

        assert((std::find(relationsParent.children.begin(), relationsParent.children.end(), a_entity) == relationsParent.children.end()));

        relations.parent = a_entity;
        relationsParent.children.emplace_back(*this);
    }
}
