#include <GJGOpch.hpp>

#include <entt/entity/registry.hpp>

#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/app.hpp>
#include <GJGO/components.hpp>
#include <GJGO/scene.hpp>
#include <GJGO/scene_management_layer.hpp>
#include <GJGO/window.hpp>

namespace GJGO
{
    SceneManagementLayer::SceneManagementLayer()
    {
        GJGO_PROFILE_SCOPE();

        this->name = "Scene Management Layer";
    }

    void SceneManagementLayer::draw()
    {
        GJGO_PROFILE_FUNCTION();

        if (App::instance->currentScene->primaryCamera)
        {
            auto view = App::instance->currentScene->m_registry.view<Transform2DComponent, SpriteComponent>();
            for (const entt::entity l_entity : view)
            {
                auto[transform, sprite] = view.get<Transform2DComponent, SpriteComponent>(l_entity);

                Renderer::begin2D(Renderer::defaultSpriteShader, *App::instance->currentScene->primaryCamera, GJGO::Window::getWidth(), GJGO::Window::getHeight());
                Renderer::drawQuad(transform.position, transform.size, transform.rotation, sprite.color, sprite.texture);
            }
        }
    }
}
