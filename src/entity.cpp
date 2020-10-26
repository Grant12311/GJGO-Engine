#include <GJGO/entity.hpp>

namespace GJGO
{
    Entity::Entity(GJGO::Application* const a_appPtr) :
        m_registryPtr(&a_appPtr->registry), m_entity(m_registryPtr->create()) {}

    Entity::~Entity()
    {
        this->m_registryPtr->destroy(this->m_entity);
    }
}
