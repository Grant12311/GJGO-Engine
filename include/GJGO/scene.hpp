#ifndef GJGO_SCENE_H
#define GJGO_SCENE_H

#include <string>

#include <entt/entity/registry.hpp>

#include <GJGO/2D/camera2D.hpp>

namespace GJGO
{
    class Entity;

    class Scene
    {
    public:
        Camera2D* primaryCamera;
    public:
        Entity createEntity(const std::string &a_tag = "");
        void destroyEntity(const Entity &a_entity);

        friend class SceneManagementLayer;
        friend class Entity;
    private:
        entt::registry m_registry;
    };
}

#endif
