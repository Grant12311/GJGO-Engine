#ifndef GJGO_ENTITY_H
#define GJGO_ENTITY_H

#include <entt/entity/registry.hpp>

#include <GJGO/app.hpp>

namespace GJGO
{
    class Entity
    {
    public:
        Entity(GJGO::Application* const a_appPtr);
        ~Entity();

        template<typename T, typename... ARGS>
        void addComponent(ARGS... a_args)
        {
            this->m_registryPtr->emplace<T>(this->m_entity, a_args...);
        }

        template<typename T>
        void removeComponent()
        {
            this->m_registryPtr->remove<T>(this->m_entity);
        }

        template<typename T>
        const T& getComponent()
        {
            return this->m_registryPtr->get<T>(this->m_entity);
        }

        template<typename T>
        bool hasComponent()
        {
            return this->m_registryPtr->has<T>(this->m_entity);
        }

        template<typename T>
        T& getComponentAccess()
        {
            return this->m_registryPtr->get<T>(this->m_entity);
        }

        template<typename T, typename... ARGS>
        void editComponent(ARGS... a_args)
        {
            this->m_registryPtr->replace<T>(this->m_entity, a_args...);
        }

        template<typename T, typename... ARGS>
        void addOrEditComponent(ARGS... a_args)
        {
            this->m_registryPtr->emplace_or_replace<T>(this->m_entity, a_args...);
        }
    private:
        entt::registry* m_registryPtr;
        entt::entity m_entity;
    };
}

#endif
