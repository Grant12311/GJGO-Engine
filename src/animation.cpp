#include <tweeny.h>

#include <GJGO/animation.hpp>
#include <GJGO/app.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    void AnimationPosition2D::step()
    {
        if (!this->paused)
        {
            this->m_targetPosPtr->x = this->m_tweenX.step(static_cast<int>(Window::deltaTime));
            this->m_targetPosPtr->y = this->m_tweenY.step(static_cast<int>(Window::deltaTime));
        }
    }

    AnimationPosition2D::AnimationPosition2D(const unsigned int a_time, Position2D &a_posRef, const Position2D &a_targetPos) :
        m_targetPosPtr(&a_posRef), m_tweenX(tweeny::from(a_posRef.x).to(a_targetPos.x).during(a_time)), m_tweenY(tweeny::from(a_posRef.y).to(a_targetPos.y).during(a_time)) {}

    AnimationPosition2D::AnimationPosition2D()
    {
        this->paused = true;
    }
}
