#include <memory>

#include <GLES3/gl31.h>

#include <glm/mat4x4.hpp>

#include <Druid/shader.h>
#include <Druid/vao.h>
#include <Druid/vbo.h>
#include <Druid/ibo.h>

#include <GJGO/color.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/2D/camera2D.hpp>
#include <GJGO/2D/transform2D.hpp>

namespace GJGO
{
    namespace Renderer
    {
        static std::unique_ptr<Druid::VAO> vaoPtr;
        static std::unique_ptr<Druid::VBO> vboPtr;
        static std::unique_ptr<Druid::IBO> iboPtr;

        Druid::Shader* currentShader;
        glm::mat4 orthoMatrix;

        static glm::mat4 genTransformer2D(const Position2D &a_position, const Size2D &a_size, const float a_rotation)
        {
            GJGO_PROFILE_FUNCTION();

            glm::mat4 toReturn = glm::translate(glm::mat4(1.0f), glm::vec3(a_position.x, a_position.y, 0.0f));

            toReturn = glm::translate(toReturn, glm::vec3(a_size.width / 2.0f, a_size.height / 2.0f, 0.0f));
            toReturn = glm::rotate(toReturn, glm::radians(a_rotation * -1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            toReturn = glm::translate(toReturn, glm::vec3((a_size.width / 2.0f) * -1.0f, (a_size.height / 2.0f) * -1.0f, 0.0f));

            return glm::scale(toReturn, glm::vec3(static_cast<float>((1 + a_size.width)) / 1.0f, static_cast<float>((1 + a_size.height)) / 1.0f, 1.0f));
        }

        void begin2D(Druid::Shader* const a_shader, const Camera2D &a_camera, const unsigned int a_width, const unsigned int a_height)
        {
            GJGO_PROFILE_FUNCTION();

            currentShader = a_shader;
            currentShader->bind();

            orthoMatrix = glm::ortho(0.0f, static_cast<float>(a_width), 0.0f, static_cast<float>(a_height));
            orthoMatrix = glm::translate(orthoMatrix, glm::vec3(a_camera.position.x * -1.0f, a_camera.position.y * -1.0f, 0.0f));

            currentShader->fillUniform("orthoMatrix", 1, false, orthoMatrix);
            glViewport(0, 0, a_width, a_height);
        }

        void drawQuad(const Position2D &a_position, const Size2D &a_size, const float a_rotation, const Color3 a_color, const unsigned int a_texID)
        {
            GJGO_PROFILE_FUNCTION();

            vaoPtr->bind();

            if (a_texID)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, a_texID);
                currentShader->fillUniform("useTexture", true);
            }

            currentShader->fillUniform("transformer", 1, false, genTransformer2D(a_position, a_size, a_rotation));
            currentShader->fillUniform("quadColor", a_color.red, a_color.green, a_color.blue);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }

        void init2D()
        {
            GJGO_PROFILE_FUNCTION();

            std::array<float, 16> quadVertices = {
                0, 0, 0.0f, 1.0f, // lower left
                0, 1, 0.0f, 0.0f, // upper left
                1, 1, 1.0f, 0.0f, // upper right
                1, 0, 1.0f, 1.0f  // lower right
            };
            std::array<unsigned int, 6> quadIndices = {
                0, 1, 2,
                0, 3, 2
            };

            vaoPtr = std::make_unique<Druid::VAO>();
            vboPtr = std::make_unique<Druid::VBO>();
            iboPtr = std::make_unique<Druid::IBO>();

            vaoPtr->bind();
            vboPtr->bind();
            iboPtr->bind();

            vboPtr->fill(quadVertices.size() * sizeof(float), quadVertices.data(), GL_STATIC_DRAW);
            iboPtr->fill(quadIndices.size() * sizeof(unsigned int), quadIndices.data(), GL_STATIC_DRAW);

            vaoPtr->setAttrib(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            vaoPtr->setAttrib(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 2 * sizeof(float));
        }
    }
}
