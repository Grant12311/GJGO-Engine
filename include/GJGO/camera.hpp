#ifndef GJGO_CAMERA2D_H
#define GJGO_CAMERA2D_H

#include <cassert>

#include <GJGO/components.hpp>
#include <GJGO/entity.hpp>

namespace GJGO
{
    class Camera
    {
    public:
        static void makePrimary(const GJGO::Entity a_entity)
        {
            assert(a_entity.hasComponent<Transform2DComponent>());
            assert(a_entity.hasComponent<Camera2DComponent>());

            m_entity = a_entity;
        }

        [[nodiscard]]
        static Entity getPrimary()
        {
            return m_entity;
        }
    private:
        inline static Entity m_entity;
    };
}

#endif
