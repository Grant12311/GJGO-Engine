#include <pch.hpp>

#include <GJGO/app.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/profiler.hpp>

namespace GJGO
{
    Entity::Entity() :
        m_entity(g_appInstancePtr->registry.create()) {}

    Entity::~Entity()
    {
        GJGO_PROFILE_FUNCTION();

        auto curr = g_appInstancePtr->registry.version(this->m_entity);
        g_appInstancePtr->registry.destroy(this->m_entity, ++curr);
    }
}
