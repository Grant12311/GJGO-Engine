#include <iostream>
#include <memory>

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
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
        static int maxTextureUnits;

        static Druid::VAO* quadVao;
        static Druid::VBO* quadVbo;
        static Druid::IBO* quadIbo;

        static Druid::Shader* spriteShader;
        static Druid::Shader* batchSpriteShader;

        static glm::mat4 orthoMatrix;

        [[nodiscard]]
        static glm::mat4 genTransformer2D(const glm::vec3 &a_position, const glm::vec2 &a_size, const float a_rotation)
        {
            glm::mat4 toReturn = glm::translate(glm::mat4(1.0f), a_position);

            toReturn = glm::translate(toReturn, glm::vec3(a_size.x / 2.0f, a_size.y / 2.0f, 0.0f));
            toReturn = glm::rotate(toReturn, glm::radians(a_rotation * -1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            toReturn = glm::translate(toReturn, glm::vec3((a_size.x / 2.0f) * -1.0f, (a_size.y / 2.0f) * -1.0f, 0.0f));

            return glm::scale(toReturn, glm::vec3((1.0f + a_size.x) / 1.0f, (1.0f + a_size.y) / 1.0f, 1.0f));
        }

        void begin2D(const Camera2D &a_camera, const unsigned int a_width, const unsigned int a_height)
        {
            orthoMatrix = glm::ortho(0.0f, static_cast<float>(a_width), 0.0f, static_cast<float>(a_height), -100.0f, 100.0f);
            orthoMatrix = glm::translate(orthoMatrix, glm::vec3(a_camera.position.x * -1.0f, a_camera.position.y * -1.0f, 0.0f));

            batchSpriteShader->bind();
            batchSpriteShader->fillUniform("orthoMatrix", false, orthoMatrix);

            spriteShader->bind();
            spriteShader->fillUniform("orthoMatrix", false, orthoMatrix);
            glViewport(0, 0, a_width, a_height);
        }

        void drawQuad(const glm::vec3 &a_position, const glm::vec2 &a_size, const float a_rotation, const glm::vec4 &a_color, GJGO::Texture* const a_texture)
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

            spriteShader->fillUniform("transformer", false, a_transform);
            spriteShader->fillUniform("quadColor", a_color.r, a_color.g, a_color.b, a_color.a);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            numDrawCallsPerFrame++;
        }

        void init2D()
        {
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);

            constexpr std::array<float, 20> quadVertices = {
                0, 0, 0, 0.0f, 1.0f, // lower left
                0, 1, 0, 0.0f, 0.0f, // upper left
                1, 1, 0, 1.0f, 0.0f, // upper right
                1, 0, 0, 1.0f, 1.0f  // lower right
            };
            constexpr std::array<unsigned int, 6> quadIndices = {
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

            quadVao->setAttrib(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
            quadVao->setAttrib(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 3 * sizeof(float));

            constexpr const char* spriteShaderVertexSource = "#version 400 core\n"
                "precision highp float;"
                "layout (location = 0) in vec3 aPos;"
                "layout (location = 1) in vec2 aTexCoord;"

                "out vec2 ourTexCoord;"

                "uniform mat4 transformer;"
                "uniform mat4 orthoMatrix;"

                "void main()"
                "{"
                "    ourTexCoord = aTexCoord;"

                "    gl_Position = orthoMatrix * transformer * vec4(aPos, 1.0);"
                "}";

            constexpr const char* spriteShaderFragmentSource = "#version 400 core\n"
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

            spriteShader = new Druid::Shader(Druid::loadFromString, spriteShaderVertexSource, spriteShaderFragmentSource);

            constexpr const char* batchSpriteShaderVertexSource = "#version 400 core\n"
                "precision highp float;"
                "layout (location = 0) in vec3 aPos;"
                "layout (location = 1) in vec2 aTexCoord;"
                "layout (location = 2) in vec3 aColor;"
                "layout (location = 3) in float aTexIndex;"

                "out vec2 ourTexCoord;"
                "out vec3 ourColor;"
                "out float ourTexIndex;"

                "uniform mat4 orthoMatrix;"

                "void main()"
                "{"
                "    ourTexCoord = aTexCoord;"
                "    ourColor = aColor;"
                "    ourTexIndex = aTexIndex;"

                "    gl_Position = orthoMatrix * vec4(aPos, 1.0);"
                "}";

            constexpr const char* batchSpriteShaderFragmentSource = "#version 400 core\n"
                "precision highp float;"
                "layout (location = 0) out vec4 color;"
                "in vec2 ourTexCoord;"
                "in vec3 ourColor;"
                "in float ourTexIndex;"

                "uniform sampler2D textures[16];"

                "void main()"
                "{"
                "    if (ourTexIndex != -1.0)"
                "        color = texture(textures[int(ourTexIndex)], ourTexCoord) * vec4(ourColor, 1.0);"
                "    else"
                "        color = vec4(ourColor, 1.0);"
                "}";

            batchSpriteShader = new Druid::Shader(Druid::loadFromString, batchSpriteShaderVertexSource, batchSpriteShaderFragmentSource);

            batchSpriteShader->bind();

            for (int i = 0; i < 16; i++)
            {
                batchSpriteShader->fillUniform((std::string("textures[") + std::to_string(i) + std::string("]")).c_str(), i);
            }
        }

        void shutdown2D()
        {
            delete spriteShader;
            delete batchSpriteShader;
        }

        Batch2D::Batch2D(const unsigned int a_capacity)
        {
            std::fill(this->textures.begin(), this->textures.end(), nullptr);

            this->m_vao.bind();
            this->m_vbo.bind();
            this->m_ibo.bind();

            this->m_vao.setAttrib(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
            this->m_vao.setAttrib(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 3 * sizeof(float));
            this->m_vao.setAttrib(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 5 * sizeof(float));
            this->m_vao.setAttrib(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 8 * sizeof(float));

            this->m_vbo.fill(36 * sizeof(float) * a_capacity, nullptr, GL_DYNAMIC_DRAW);
            this->m_ibo.fill(6 * sizeof(unsigned int) * a_capacity, nullptr, GL_DYNAMIC_DRAW);
        }

        [[nodiscard]]
        size_t Batch2D::size() const
        {
            return this->m_vertices.size() / 36;
        }

        void Batch2D::clear()
        {
            this->m_vertices.clear();
            this->m_indices.clear();
        }

        void Batch2D::addQuad(const glm::vec3 &a_position, const glm::vec2 &a_size, const float a_rotation, const glm::vec4 &a_color, const float a_textureIndex)
        {
            this->addQuad(genTransformer2D(a_position, a_size, a_rotation), a_color, a_textureIndex);
        }

        void Batch2D::addQuad(const glm::mat4 &a_transform, const glm::vec4 &a_color, const float a_textureIndex)
        {
            std::array<float, 36> quadVertices = {
                0, 0, 0, 0.0f, 1.0f, a_color.r, a_color.g, a_color.b, a_textureIndex, // lower left
                0, 1, 0, 0.0f, 0.0f, a_color.r, a_color.g, a_color.b, a_textureIndex, // upper left
                1, 1, 0, 1.0f, 0.0f, a_color.r, a_color.g, a_color.b, a_textureIndex, // upper right
                1, 0, 0, 1.0f, 1.0f, a_color.r, a_color.g, a_color.b, a_textureIndex  // lower right
            };
            const std::array<unsigned int, 6> quadIndices = {
                0 + 4 * static_cast<unsigned int>(this->size()), 1 + 4 * static_cast<unsigned int>(this->size()), 2 + 4 * static_cast<unsigned int>(this->size()),
                0 + 4 * static_cast<unsigned int>(this->size()), 3 + 4 * static_cast<unsigned int>(this->size()), 2 + 4 * static_cast<unsigned int>(this->size())
            };

            std::array<glm::vec4, 4> transformedVertexPositions;
            transformedVertexPositions[0] = a_transform * glm::vec4(quadVertices[0], quadVertices[1], quadVertices[2], 1.0f);
            transformedVertexPositions[1] = a_transform * glm::vec4(quadVertices[9], quadVertices[10], quadVertices[11], 1.0f);
            transformedVertexPositions[2] = a_transform * glm::vec4(quadVertices[18], quadVertices[19], quadVertices[20], 1.0f);
            transformedVertexPositions[3] = a_transform * glm::vec4(quadVertices[27], quadVertices[28], quadVertices[29], 1.0f);

            quadVertices[0] = transformedVertexPositions[0].x;
            quadVertices[1] = transformedVertexPositions[0].y;
            quadVertices[2] = transformedVertexPositions[0].z;
            quadVertices[9] = transformedVertexPositions[1].x;
            quadVertices[10] = transformedVertexPositions[1].y;
            quadVertices[11] = transformedVertexPositions[1].z;
            quadVertices[18] = transformedVertexPositions[2].x;
            quadVertices[19] = transformedVertexPositions[2].y;
            quadVertices[20] = transformedVertexPositions[2].z;
            quadVertices[27] = transformedVertexPositions[3].x;
            quadVertices[28] = transformedVertexPositions[3].y;
            quadVertices[29] = transformedVertexPositions[3].z;

            this->m_vertices.insert(this->m_vertices.end(), quadVertices.begin(), quadVertices.end());
            this->m_indices.insert(this->m_indices.end(), quadIndices.begin(), quadIndices.end());
        }

        void Batch2D::draw()
        {
            batchSpriteShader->bind();
            this->m_vao.bind();

            for (unsigned int i = 0; i < this->textures.size(); i++)
            {
                if (this->textures[i] != nullptr)
                {
                    this->textures[i]->bind(i);
                }
            }

            glBufferSubData(GL_ARRAY_BUFFER, 0, this->m_vertices.size() * sizeof(float), this->m_vertices.data());
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->m_indices.size() * sizeof(float), this->m_indices.data());

            glDrawElements(GL_TRIANGLES, static_cast<int>(this->m_indices.size()), GL_UNSIGNED_INT, nullptr);
            numDrawCallsPerFrame++;

            spriteShader->bind();
        }
    }
}
