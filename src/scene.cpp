#include <GJGO/components.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/scene.hpp>

namespace GJGO
{
    Entity Scene::createEntity(const std::string &a_tag)
    {
        GJGO_PROFILE_FUNCTION();

        Entity created(this->m_registry.create(), this);
        created.addComponent<TagComponent>(a_tag);
        return created;
    }

    void Scene::destroyEntity(const Entity &a_entity)
    {
        GJGO_PROFILE_FUNCTION();

        this->m_registry.destroy(a_entity.m_entity);
    }
}
