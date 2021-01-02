#ifndef GJGO_ANIMATION_H
#define GJGO_ANIMATION_H

#include <tweeny.h>

#include <GJGO/2D/transform2D.hpp>

namespace GJGO
{
    class Animation
    {
    public:
        bool paused = false;

        virtual void step() = 0;
        virtual ~Animation() = default;
    };

    class AnimationPosition2D : public Animation
    {
    private:
        Position2D* m_targetPosPtr;
        tweeny::tween<int> m_tweenX, m_tweenY;
    public:
        virtual void step() override;
        AnimationPosition2D(const unsigned int a_time, Position2D &a_posRef, const Position2D &a_targetPos);
        AnimationPosition2D();
    };
}

#endif
