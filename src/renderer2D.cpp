#include <iostream>
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
        static Druid::Shader* batchSpriteShader;

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
            orthoMatrix = glm::ortho(0.0f, static_cast<float>(a_width), 0.0f, static_cast<float>(a_height));
            orthoMatrix = glm::translate(orthoMatrix, glm::vec3(a_camera.position.x * -1.0f, a_camera.position.y * -1.0f, 0.0f));

            batchSpriteShader->bind();
            batchSpriteShader->fillUniform("orthoMatrix", 1, false, orthoMatrix);

            spriteShader->bind();
            spriteShader->fillUniform("orthoMatrix", 1, false, orthoMatrix);
            glViewport(0, 0, a_width, a_height);
        }

        void drawQuad(const glm::vec2 &a_position, const glm::vec2 &a_size, const float a_rotation, const glm::vec4 &a_color, GJGO::Texture* const a_texture)
        {
            drawQuad(genTransformer2D(a_position, a_size, a_rotation), a_color, a_texture);
        }

        void drawQuad(const glm::mat4 &a_transform, const glm::vec4 &a_color, GJGO::Texture* const a_texture)
        {
            quadVao->bind();

            if (a_texture)
            {
                a_texture->bind();
                spriteShader->fillUniform("useTexture", true);
            }else{
                spriteShader->fillUniform("useTexture", false);
            }

            spriteShader->fillUniform("transformer", 1, false, a_transform);
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

            const char* const spriteShaderVertexSource = "#version 330 core\n"
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

            const char* const spriteShaderFragmentSource = "#version 330 core\n"
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

            spriteShader = new Druid::Shader(spriteShaderVertexSource, spriteShaderFragmentSource);

            const char* const batchSpriteShaderVertexSource = "#version 330 core\n"
                "precision highp float;"
                "layout (location = 0) in vec2 aPos;"
                "layout (location = 1) in vec2 aTexCoord;"
                "layout (location = 2) in vec3 aColor;"

                "out vec2 ourTexCoord;"
                "out vec3 ourColor;"

                "uniform mat4 orthoMatrix;"

                "void main()"
                "{"
                "    ourTexCoord = aTexCoord;"
                "    ourColor = aColor;"

                "    gl_Position = orthoMatrix * vec4(aPos, 1.0, 1.0);"
                "}";

            const char* const batchSpriteShaderFragmentSource = "#version 330 core\n"
                "precision highp float;"
                "layout (location = 0) out vec4 color;"
                "in vec2 ourTexCoord;"
                "in vec3 ourColor;"

                "uniform sampler2D texture1;"
                "uniform bool useTexture;"

                "void main()"
                "{"
                "    if (useTexture)"
                "        color = texture(texture1, ourTexCoord) * vec4(ourColor, 1.0);"
                "    else"
                "        color = vec4(ourColor, 1.0);"
                "}";

            batchSpriteShader = new Druid::Shader(batchSpriteShaderVertexSource, batchSpriteShaderFragmentSource);
        }

        void shutdown2D()
        {
            delete quadVao;
            delete quadVbo;
            delete quadIbo;

            delete spriteShader;
            delete batchSpriteShader;
        }

        Batch2D::Batch2D()
        {
            this->m_vao.bind();
            this->m_vbo.bind();
            this->m_ibo.bind();

            this->m_vao.setAttrib(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
            this->m_vao.setAttrib(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 2 * sizeof(float));
            this->m_vao.setAttrib(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 4 * sizeof(float));
        }

        size_t Batch2D::size() const
        {
            return this->m_vertices.size() / 28;
        }

        void Batch2D::clear()
        {
            this->m_vertices.clear();
            this->m_indices.clear();
        }

        void Batch2D::addQuad(const glm::vec2 &a_position, const glm::vec2 &a_size, const float a_rotation, const glm::vec4 &a_color, GJGO::Texture* const a_texture)
        {
            this->addQuad(genTransformer2D(a_position, a_size, a_rotation), a_color, a_texture);
        }

        void Batch2D::addQuad(const glm::mat4 &a_transform, const glm::vec4 &a_color, GJGO::Texture* const a_texture)
        {
            std::array<float, 28> quadVertices = {
                0, 0, 0.0f, 1.0f, a_color.r, a_color.g, a_color.b, // lower left
                0, 1, 0.0f, 0.0f, a_color.r, a_color.g, a_color.b, // upper left
                1, 1, 1.0f, 0.0f, a_color.r, a_color.g, a_color.b, // upper right
                1, 0, 1.0f, 1.0f, a_color.r, a_color.g, a_color.b  // lower right
            };
            std::array<unsigned int, 6> quadIndices = {
                0 + 4 * this->size(), 1 + 4 * this->size(), 2 + 4 * this->size(),
                0 + 4 * this->size(), 3 + 4 * this->size(), 2 + 4 * this->size()
            };

            std::array<glm::vec4, 4> transformedVertexPositions;
            transformedVertexPositions[0] = a_transform * glm::vec4(quadVertices[0], quadVertices[1], 1.0f, 1.0f);
            transformedVertexPositions[1] = a_transform * glm::vec4(quadVertices[7], quadVertices[8], 1.0f, 1.0f);
            transformedVertexPositions[2] = a_transform * glm::vec4(quadVertices[14], quadVertices[15], 1.0f, 1.0f);
            transformedVertexPositions[3] = a_transform * glm::vec4(quadVertices[21], quadVertices[22], 1.0f, 1.0f);

            quadVertices[0] = transformedVertexPositions[0].x;
            quadVertices[1] = transformedVertexPositions[0].y;
            quadVertices[7] = transformedVertexPositions[1].x;
            quadVertices[8] = transformedVertexPositions[1].y;
            quadVertices[14] = transformedVertexPositions[2].x;
            quadVertices[15] = transformedVertexPositions[2].y;
            quadVertices[21] = transformedVertexPositions[3].x;
            quadVertices[22] = transformedVertexPositions[3].y;

            this->m_vertices.insert(this->m_vertices.end(), quadVertices.begin(), quadVertices.end());
            this->m_indices.insert(this->m_indices.end(), quadIndices.begin(), quadIndices.end());
        }

        void Batch2D::draw()
        {
            batchSpriteShader->bind();
            this->m_vao.bind();

            this->m_vbo.fill(this->m_vertices.size() * sizeof(float), this->m_vertices.data(), GL_STATIC_DRAW);
            this->m_ibo.fill(this->m_indices.size() * sizeof(unsigned int), this->m_indices.data(), GL_STATIC_DRAW);

            /*if (a_texture)
            {
                a_texture->bind();
                spriteShader->fillUniform("useTexture", true);
            }else{
                spriteShader->fillUniform("useTexture", false);
            }*/

            batchSpriteShader->fillUniform("useTexture", false);
            glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_INT, nullptr);

            spriteShader->bind();
        }
    }
}
