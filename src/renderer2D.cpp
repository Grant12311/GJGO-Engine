#include <memory>

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <Druid/ibo.h>
#include <Druid/vao.h>
#include <Druid/vbo.h>

#include <GJGO/camera2D.hpp>
#include <GJGO/renderer2D.hpp>

namespace GJGO
{
    namespace Renderer
    {
        static Druid::VAO* quadVao;
        static Druid::VBO* quadVbo;
        static Druid::IBO* quadIbo;
        static Druid::Shader* spriteShader;

        static glm::mat4 orthoMatrix;

        static glm::mat4 genTransformer2D(const glm::vec2 &a_position, const glm::vec2 &a_size, const float a_rotation)
        {
            glm::mat4 toReturn = glm::translate(glm::mat4(1.0f), glm::vec3(a_position, 0.0f));

            toReturn = glm::translate(toReturn, glm::vec3(a_size.x / 2.0f, a_size.y / 2.0f, 0.0f));
            toReturn = glm::rotate(toReturn, glm::radians(a_rotation * -1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            toReturn = glm::translate(toReturn, glm::vec3((a_size.x / 2.0f) * -1.0f, (a_size.y / 2.0f) * -1.0f, 0.0f));

            return glm::scale(toReturn, glm::vec3((1.0f + a_size.x) / 1.0f, (1.0f + a_size.y) / 1.0f, 1.0f));
        }

        void begin2D(const Camera2D &a_camera, const unsigned int a_width, const unsigned int a_height)
        {
            spriteShader->bind();

            orthoMatrix = glm::ortho(0.0f, static_cast<float>(a_width), 0.0f, static_cast<float>(a_height));
            orthoMatrix = glm::translate(orthoMatrix, glm::vec3(a_camera.position.x * -1.0f, a_camera.position.y * -1.0f, 0.0f));

            spriteShader->fillUniform("orthoMatrix", 1, false, orthoMatrix);
            glViewport(0, 0, a_width, a_height);
        }

        void drawQuad(const glm::vec2 &a_position, const glm::vec2 &a_size, const float a_rotation, const glm::vec4 &a_color, const unsigned int a_texID)
        {
            quadVao->bind();

            if (a_texID)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, a_texID);
                spriteShader->fillUniform("useTexture", true);
            }else{
                spriteShader->fillUniform("useTexture", false);
            }

            spriteShader->fillUniform("transformer", 1, false, genTransformer2D(a_position, a_size, a_rotation));
            spriteShader->fillUniform("quadColor", a_color.r, a_color.g, a_color.b, a_color.a);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }

        void init2D()
        {
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

            quadVao = new Druid::VAO;
            quadVbo = new Druid::VBO;
            quadIbo = new Druid::IBO;

            quadVao->bind();
            quadVbo->bind();
            quadIbo->bind();

            quadVbo->fill(quadVertices.size() * sizeof(float), quadVertices.data(), GL_STATIC_DRAW);
            quadIbo->fill(quadIndices.size() * sizeof(unsigned int), quadIndices.data(), GL_STATIC_DRAW);

            quadVao->setAttrib(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            quadVao->setAttrib(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 2 * sizeof(float));

            const char* const defaultSpriteShaderVertexSource = "#version 330 core\n"
                "precision highp float;"
                "layout (location = 0) in vec2 aPos;"
                "layout (location = 1) in vec2 aTexCoord;"

                "out vec2 ourTexCoord;"

                "uniform mat4 transformer;"
                "uniform mat4 orthoMatrix;"

                "void main()"
                "{"
                "    ourTexCoord = aTexCoord;"

                "    gl_Position = orthoMatrix * transformer * vec4(aPos, 1.0, 1.0);"
                "}";

            const char* const defaultSpriteShaderFragmentSource = "#version 330 core\n"
                "precision highp float;"
                "layout (location = 0) out vec4 color;"
                "in vec2 ourTexCoord;"

                "uniform vec4 quadColor;"
                "uniform sampler2D texture1;"
                "uniform bool useTexture;"

                "void main()"
                "{"
                "    if (useTexture)"
                "        color = texture(texture1, ourTexCoord) * quadColor;"
                "    else"
                "        color = quadColor;"
                "}";

            spriteShader = new Druid::Shader(defaultSpriteShaderVertexSource, defaultSpriteShaderFragmentSource);
        }

        void shutdown2D()
        {
            delete quadVao;
            delete quadVbo;
            delete quadIbo;

            delete spriteShader;
        }
    }
}