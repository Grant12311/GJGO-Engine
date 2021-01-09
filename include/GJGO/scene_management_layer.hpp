#ifndef GJGO_SCENE_MANAGEMENT_LAYER_H
#define GJGO_SCENE_MANAGEMENT_LAYER_H

#include <GJGO/layer.hpp>
#include <GJGO/profiler.hpp>

namespace GJGO
{
    class SceneManagementLayer : public Layer
    {
    public:
        SceneManagementLayer();

        virtual void draw() override;
    };
}

#endif
