#ifndef GJGO_ENTITY_H
#define GJGO_ENTITY_H

#include <entt/entity/registry.hpp>

#include <GJGO/app.hpp>

namespace GJGO
{
    class Entity
    {
    public:
        Entity();
        ~Entity();

        template<typename T, typename... ARGS>
        void addComponent(ARGS... a_args)
        {
            g_appInstancePtr->registry.emplace<T>(this->m_entity, a_args...);
        }

        template<typename T>
        void removeComponent()
        {
            g_appInstancePtr->registry.remove<T>(this->m_entity);
        }

        template<typename T>
        const T& getComponent()
        {
            return g_appInstancePtr->registry.get<T>(this->m_entity);
        }

        template<typename T>
        bool hasComponent()
        {
            return g_appInstancePtr->registry.has<T>(this->m_entity);
        }

        template<typename T>
        T& getComponentAccess()
        {
            return g_appInstancePtr->registry.get<T>(this->m_entity);
        }

        template<typename T, typename... ARGS>
        void editComponent(ARGS... a_args)
        {
            g_appInstancePtr->registry.replace<T>(this->m_entity, a_args...);
        }

        template<typename T, typename... ARGS>
        void addOrEditComponent(ARGS... a_args)
        {
            g_appInstancePtr->registry.emplace_or_replace<T>(this->m_entity, a_args...);
        }
    private:
        entt::entity m_entity;
    };
}

#endif
