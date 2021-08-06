#include <algorithm>
#include <array>
#include <string>
#include <vector>

#include <GJGO/gjgo.hpp>

class GameLayer final : public GJGO::Layer
{
public:
    GJGO::Entity e1;
    GJGO::Entity e2;

    GameLayer() :
        e1(GJGO::Entity::create("e1")), e2(GJGO::Entity::create("e2"))
    {
        this->name = "Game Layer";

        e1.addComponent<GJGO::Transform2DComponent>(glm::vec3(100.0, 100.0, 0.0), glm::vec2(100.0, 100.0));
        e2.addComponent<GJGO::Transform2DComponent>(glm::vec3(215.0, 215.0, 0.0), glm::vec2(100.0, 100.0));

        e1.addComponent<GJGO::SpriteComponent>();
        e2.addComponent<GJGO::SpriteComponent>();

        e1.addComponent<GJGO::RigidBody2DComponent>([](const GJGO::Entity a_e1, const GJGO::Entity a_e2)
        {
            std::cout << a_e1.getComponent<GJGO::TagComponent>().name << " <-> " << a_e2.getComponent<GJGO::TagComponent>().name << '\n';
        });

        e2.addComponent<GJGO::RigidBody2DComponent>();
    }

    virtual void onUpdate() override
    {
        static double timePassed = 0.0;
        static std::vector<double> times;

        if (timePassed >= 100.0)
        {
            double total = 0.0;
            for (const double l_dt : times)
            {
                total += l_dt;
            }

            GJGO::Window::setTitle((std::to_string(1.0 / (total / static_cast<double>(times.size())) * 1000.0) + std::string(" - ") + std::to_string(GJGO::Renderer::numDrawCallsPerFrame)).c_str());

            timePassed = 0.0;
            times.clear();
        }else{
            times.emplace_back(GJGO::App::instance->deltaTime);
            timePassed += GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_W))
        {
            e1.getComponent<GJGO::Transform2DComponent>().position.y += 1.0 * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_S))
        {
            e1.getComponent<GJGO::Transform2DComponent>().position.y -= 1.0 * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_A))
        {
            e1.getComponent<GJGO::Transform2DComponent>().position.x -= 1.0 * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_D))
        {
            e1.getComponent<GJGO::Transform2DComponent>().position.x += 1.0 * GJGO::App::instance->deltaTime;
        }
    }

    virtual void onEvent(const GJGO::Event &a_event) override
    {
        switch (a_event.type)
        {
            case GJGO::EventType::keyDown:
                switch (a_event.keycode)
                {
                    case GLFW_KEY_EQUAL:
                        GJGO::Renderer::useBatchRendererAsDefault = true;
                        break;
                    case GLFW_KEY_MINUS:
                        GJGO::Renderer::useBatchRendererAsDefault = false;
                        break;
                }
                break;
            default:
                break;
        }
    }

    virtual void draw() override
    {

    }
};

int main()
{
    GJGO::AppSettings settings;
    settings.windowWidth = 768;
    settings.windowHeight = 432;

    GJGO::App app(settings);

    app.layers.emplace_back(new GameLayer);

    app.run();

    return 0;
}
