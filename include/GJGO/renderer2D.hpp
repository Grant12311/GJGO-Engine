#ifndef GJGO_RENDERER2D_H
#define GJGO_RENDERER2D_H

#include <Druid/ibo.h>
#include <Druid/shader.h>
#include <Druid/vao.h>
#include <Druid/vbo.h>

#include <GJGO/camera2D.hpp>
#include <GJGO/texture.hpp>

namespace GJGO
{
    namespace Renderer
    {
        inline int numDrawCallsPerFrame = 0;
        inline bool useBatchRendererAsDefault = true;

        void begin2D(const Camera2D &a_camera, const unsigned int a_width, const unsigned int a_height);

        void drawQuad(const glm::vec3 &a_position, const glm::vec2 &a_size, const float a_rotation = 0.0f, const glm::vec4 &a_color = {1.0f, 1.0f, 1.0f, 1.0f}, GJGO::Texture* const a_texture = nullptr);
        void drawQuad(const glm::mat4 &a_transform, const glm::vec4 &a_color = {1.0f, 1.0f, 1.0f, 1.0f}, GJGO::Texture* const a_texture = nullptr);

        void init2D();
        void shutdown2D();

        class Batch2D
        {
        public:
            std::array<Texture2D*, 16> textures;

            Batch2D(const unsigned int a_capacity);

            [[nodiscard]]
            size_t size() const;
            void clear();

            void addQuad(const glm::vec3 &a_position, const glm::vec2 &a_size, const float a_rotation = 0.0f, const glm::vec4 &a_color = {1.0f, 1.0f, 1.0f, 1.0f}, const float a_textureIndex = -1.0f);
            void addQuad(const glm::mat4 &a_transform, const glm::vec4 &a_color = {1.0f, 1.0f, 1.0f, 1.0f}, const float a_textureIndex = -1.0f);

            void draw();
        private:
            Druid::VAO m_vao;
            Druid::IBO m_ibo;
            Druid::VBO m_vbo;

            std::vector<float> m_vertices;
            std::vector<unsigned int> m_indices;
        };
    }
}

#endif
