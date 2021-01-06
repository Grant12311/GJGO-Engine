#include <GJGOpch.hpp>

#include <GJGO/animation.hpp>
#include <GJGO/app.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    void AnimationPosition2D::step()
    {
        if (!this->paused && !this->m_done)
        {
            this->m_targetPosPtr->x = this->m_tweenX.step(static_cast<int>(Window::deltaTime));
            this->m_targetPosPtr->y = this->m_tweenY.step(static_cast<int>(Window::deltaTime));
        }
    }

    bool AnimationPosition2D::isDone()
    {
        return this->m_done;
    }

    AnimationPosition2D::AnimationPosition2D(const unsigned int a_time, Position2D &a_posRef, const Position2D &a_targetPos) :
        m_targetPosPtr(&a_posRef)
    {
        std::function<bool(tweeny::tween<int>&)> checkTweenProgress = [&](tweeny::tween<int> &a_tweenRef)
        {
            if (a_tweenRef.progress() >= 1.0f)
            {
                this->m_done = true;
            }

            return false;
        };

        this->m_tweenX = tweeny::from(a_posRef.x).to(a_targetPos.x).during(a_time).onStep(checkTweenProgress);
        this->m_tweenY = tweeny::from(a_posRef.y).to(a_targetPos.y).during(a_time).onStep(checkTweenProgress);
    }

    AnimationPosition2D::AnimationPosition2D()
    {
        this->paused = true;
    }
}
