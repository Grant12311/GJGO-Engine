#include <iostream>

#include <GLES3/gl31.h>

#include <imgui.h>

#include <Hangar2/keycodes.h>

#include <Druid/vao.h>
#include <Druid/vbo.h>
#include <Druid/ibo.h>
#include <Druid/shader.h>

#include <GJGO/app.hpp>
#include <GJGO/clipboard.hpp>
#include <GJGO/entity.hpp>
#include <GJGO/event.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/2D/transform2D.hpp>

class GameLayer : public GJGO::Layer
{
private:
    Druid::VAO m_vao;
    Druid::VBO m_vbo;
    Druid::Shader shader = Druid::Shader("sprite.shader");

    unsigned int fbo;
    unsigned int depthBuffer;
    unsigned int texture;
public:
    void onUpdate() override {}

    void onEvent(GJGO::Event* const a_event) override
    {
        switch (a_event->type)
        {
            case GJGO::EventType::keyDown:
                GJGO_LOG_INFO("Key Down: ", a_event->keycode);
                if (a_event->keycode == HGR_0){
                    GJGO::g_appInstancePtr->window.setVsync(0);
                }else if (a_event->keycode == HGR_1){
                    GJGO::g_appInstancePtr->window.setVsync(1);
                    GJGO::g_appInstancePtr->window.setFramerateCap(60);
                }else if (a_event->keycode == HGR_2){
                    GJGO::g_appInstancePtr->window.setVsync(1);
                    GJGO::g_appInstancePtr->window.setFramerateCap(30);
                }
                break;
            case GJGO::EventType::keyUp:
                GJGO_LOG_INFO("Key Up: ", a_event->keycode);
                break;
            case GJGO::EventType::mouseMove:
                GJGO_LOG_INFO("(", a_event->mousePosition.absolute.x, ", ", a_event->mousePosition.absolute.y, ")");
                break;
        }
    }

    void draw() override
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        this->m_vao.bind();
        this->shader.bind();
        glViewport(0, 0, 1600, 900);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /*void drawGui() override
    {
        ImGui::Begin("Renderer", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Image((void*)this->texture, {100, 100}, {0, 1}, {1, 0});

        ImGui::End();
    }*/

    GameLayer()
    {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1600, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glGenRenderbuffers(1, &this->depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1600, 900);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthBuffer);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            exit(1);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        this->m_vao.bind();
        this->m_vbo.bind();

        std::array<float, 6> vertices = {
             0.0f,  0.5f,
             0.5f, -0.5f,
            -0.5f, -0.5f
        };

        this->m_vbo.fill(6 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        this->m_vao.setAttrib(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);
    }
};

int main()
{
    GJGO_LOG_SET_PRINT_FILE(false);
    GJGO_LOG_SET_PRINT_FUNCTION(false);
    Hangar::Config winConfig;
    winConfig.vsync = false;
    GJGO::Application app(winConfig);

    app.layers.emplace_back(new GameLayer);

    GJGO::Entity e;
    e.addComponent<GJGO::Transform2D>();

    app.run();

    return 0;
}
