#include <GJGO/app.hpp>
#include <GJGO/entity.hpp>

namespace GJGO
{
    Entity::Entity() :
        m_entity(g_appInstancePtr->registry.create()) {}

    Entity::~Entity()
    {
        auto curr = g_appInstancePtr->registry.version(this->m_entity);
        g_appInstancePtr->registry.destroy(this->m_entity, ++curr);
    }
}
