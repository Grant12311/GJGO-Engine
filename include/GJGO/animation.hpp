#ifndef GJGO_ANIMATION_H
#define GJGO_ANIMATION_H

#include <array>
#include <cstddef>
#include <tuple>

#include <tweeny/tweeny.h>

#include <GJGO/components.hpp>

namespace GJGO
{
    class AnimationSection
    {
    public:
        virtual ~AnimationSection() = default;
        virtual void step(const double a_dt, Entity a_entity) = 0;
    };

    class AnimationSectionTransform2D final : AnimationSection
    {
    public:
        AnimationSectionTransform2D(const Transform2DComponent &a_start) :
            m_time(0.0), m_tween(tweeny::from(a_start.position.x, a_start.position.y, a_start.position.z, a_start.size.x, a_start.size.y, a_start.rotation)) {}

        virtual ~AnimationSectionTransform2D() = default;

        void addFrame(const double a_time, const Transform2DComponent &a_transform)
        {
            this->m_tween.to(a_transform.position.x, a_transform.position.y, a_transform.position.z, a_transform.size.x, a_transform.size.y, a_transform.rotation)
                         .during(a_time - this->m_time);
            this->m_time = a_time;
        }

        virtual void step(const double a_dt, Entity a_entity) override
        {
            const std::array<float, 6> data = this->m_tween.step(static_cast<unsigned int>(a_dt));

            Transform2DComponent& transform = a_entity.getComponent<Transform2DComponent>();
            transform.position = glm::vec3(data[0], data[1], data[2]);
            transform.size = glm::vec2(data[3], data[4]);
            transform.rotation = data[5];
        }
    private:
        double m_time;
        tweeny::tween<float, float, float, float, float, float> m_tween;
    };

    class Animation
    {
    public:
        Animation(const Entity a_entity) :
            m_entity(a_entity) {}

        ~Animation()
        {
            for (size_t i = 0; i < this->m_sections.size(); i++)
            {
                delete this->m_sections[i];
            }
        }

        void addSection(AnimationSection* const a_section)
        {
            this->m_sections.emplace_back(a_section);
        }
    private:
        Entity m_entity;
        std::vector<AnimationSection*> m_sections;
    };
}

#endif
