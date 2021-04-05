#ifndef GJGO_CAMERA2D_H
#define GJGO_CAMERA2D_H

#include <glm/vec3.hpp>

namespace GJGO
{
    class Camera2D
    {
    public:
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        float rotation = 0.0f;
    };
}

#endif
