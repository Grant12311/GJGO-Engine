#ifndef GJGO_ENTITY_H
#define GJGO_ENTITY_H

#include <entt/entity/registry.hpp>

#include <GJGO/app.hpp>

namespace GJGO
{
    class Entity
    {
    public:
        Entity(const entt::entity a_entity = entt::null);

        static Entity create(const std::string &a_name);
        static Entity create();

        [[nodiscard]]
        static Entity getByName(const std::string &a_name);

        bool isValid;

        [[nodiscard]]
        constexpr entt::entity getRaw() const { return this->m_entity; }

        template<typename T, typename... ARGS>
        void addComponent(ARGS... a_args)
        {
           App::instance->registry.emplace<T>(this->m_entity, a_args...);
        }

        template<typename T>
        void removeComponent()
        {
            App::instance->registry.remove<T>(this->m_entity);
        }

        template<typename T>
        [[nodiscard]]
        T& getComponent()
        {
            return App::instance->registry.get<T>(this->m_entity);
        }

        template<typename T>
        [[nodiscard]]
        const T& getComponent() const
        {
            return App::instance->registry.get<T>(this->m_entity);
        }

        template<typename T>
        [[nodiscard]]
        bool hasComponent()
        {
            return App::instance->registry.has<T>(this->m_entity);
        }

        template<typename T, typename... ARGS>
        void editComponent(ARGS... a_args)
        {
            App::instance->registry.replace<T>(this->m_entity, a_args...);
        }

        template<typename T, typename... ARGS>
        void addOrEditComponent(ARGS... a_args)
        {
            App::instance->registry.emplace_or_replace<T>(this->m_entity, a_args...);
        }
    private:
        entt::entity m_entity;
    };
}

#endif
