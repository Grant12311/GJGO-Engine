#include <GJGOpch.hpp>

#include <GJGO/app.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/profiler.hpp>

namespace GJGO
{
    Entity::Entity(const entt::entity a_entity, Scene* const a_scenePtr) :
        m_entity(a_entity), m_scenePtr(a_scenePtr) {}
}
