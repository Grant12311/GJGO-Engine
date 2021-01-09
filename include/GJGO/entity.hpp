#ifndef GJGO_ENTITY_H
#define GJGO_ENTITY_H

#include <entt/entity/registry.hpp>

#include <GJGO/app.hpp>
#include <GJGO/scene.hpp>

namespace GJGO
{
    class Entity
    {
    public:
        Entity(const entt::entity a_entity = entt::null, Scene* const a_scenePtr = nullptr);

        template<typename T, typename... ARGS>
        void addComponent(ARGS... a_args)
        {
           this->m_scenePtr->m_registry.emplace<T>(this->m_entity, a_args...);
        }

        template<typename T>
        void removeComponent()
        {
            this->m_scenePtr->m_registry.remove<T>(this->m_entity);
        }

        template<typename T>
        const T& getComponent()
        {
            return this->m_scenePtr->m_registry.get<T>(this->m_entity);
        }

        template<typename T>
        bool hasComponent()
        {
            return this->m_scenePtr->m_registry.has<T>(this->m_entity);
        }

        template<typename T>
        T& getComponentAccess()
        {
            return this->m_scenePtr->m_registry.get<T>(this->m_entity);
        }

        template<typename T, typename... ARGS>
        void editComponent(ARGS... a_args)
        {
            this->m_scenePtr->m_registry.replace<T>(this->m_entity, a_args...);
        }

        template<typename T, typename... ARGS>
        void addOrEditComponent(ARGS... a_args)
        {
            this->m_scenePtr->m_registry.emplace_or_replace<T>(this->m_entity, a_args...);
        }

        friend class Scene;
    private:
        entt::entity m_entity;
        Scene* m_scenePtr;
    };
}

#endif
