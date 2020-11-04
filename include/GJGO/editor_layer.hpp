#ifndef GJGO_EDITOR_LAYER_H
#define GJGO_EDITOR_LAYER_H

#include <GJGO/layer.hpp>

namespace GJGO
{
    class EditorLayer : public GJGO::Layer
    {
    private:
        bool showEntitiesWindow = true;
    public:
        void drawGui();
    };
}

#endif
