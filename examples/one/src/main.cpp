#include <GJGO/gjgo.hpp>

class GameLayer final : public GJGO::Layer
{
public:
    GJGO::Entity player;

    GameLayer() :
        player(GJGO::Entity::create("Plr"))
    {
        this->name = "Game Layer";

        this->player.addComponent<GJGO::Transform2DComponent>(glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 100.0f));
        this->player.addComponent<GJGO::SpriteComponent>(GJGO::Texture2D::create("res/wall.jpg", GJGO::TextureSettings::standard, GL_NEAREST, GL_NEAREST));

        GJGO::Entity trans = GJGO::Entity::create();
        trans.addComponent<GJGO::Transform2DComponent>(glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 100.0f));
        trans.addComponent<GJGO::SpriteComponent>(GJGO::Texture2D::create("res/trans.png", GJGO::TextureSettings::hasTransparency, GL_NEAREST, GL_NEAREST), glm::vec4(1.0f), 1);

        for (unsigned int i = 0; i < 10000; i++)
        {
            GJGO::Entity e = GJGO::Entity::create();
            e.addComponent<GJGO::Transform2DComponent>(glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f));
            e.addComponent<GJGO::SpriteComponent>(nullptr, glm::vec4(1.0f), 0);
        }
    }

    virtual void onUpdate() override
    {
        static double timePassed = 0.0;
        static std::vector<double> times;

        if (timePassed >= 100.0)
        {
            double total;
            for (const double l_dt : times)
            {
                total += l_dt;
            }

            GJGO::Window::setTitle((std::to_string(1.0 / (total / times.size()) * 1000.0) + std::string(" - ") + std::to_string(GJGO::Renderer::numDrawCallsPerFrame)).c_str());

            timePassed = 0.0;
            times.clear();
        }else{
            times.emplace_back(GJGO::App::instance->deltaTime);
            timePassed += GJGO::App::instance->deltaTime;
        }

        GJGO::Transform2DComponent& plrTransform = this->player.getComponent<GJGO::Transform2DComponent>();

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_W))
        {
            plrTransform.position.y += 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_S))
        {
            plrTransform.position.y -= 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_A))
        {
            plrTransform.position.x -= 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_D))
        {
            plrTransform.position.x += 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_UP))
        {
            plrTransform.size.x += 1.0f * GJGO::App::instance->deltaTime;
            plrTransform.size.y += 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_DOWN))
        {
            plrTransform.size.x -= 1.0f * GJGO::App::instance->deltaTime;
            plrTransform.size.y -= 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_RIGHT))
        {
            plrTransform.rotation += 0.1f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_LEFT))
        {
            plrTransform.rotation -= 0.1f * GJGO::App::instance->deltaTime;
        }
    }

    virtual void onEvent(const GJGO::Event &a_event) override
    {
        switch (a_event.type)
        {
            case GJGO::EventType::keyDown:
                GJGO_LOG_INFO("DOWN: ", a_event.keycode);
                switch (a_event.keycode)
                {
                    case GLFW_KEY_LEFT_BRACKET:
                        GJGO::Texture2D::get("res/wall.jpg")->bind();
                        GJGO::Texture2D::get("res/wall.jpg")->setFilters(GL_NEAREST, GL_NEAREST);
                        break;
                    case GLFW_KEY_RIGHT_BRACKET:
                        GJGO::Texture2D::get("res/wall.jpg")->bind();
                        GJGO::Texture2D::get("res/wall.jpg")->setFilters(GL_LINEAR, GL_LINEAR);
                        break;
                    case GLFW_KEY_EQUAL:
                        GJGO::Renderer::useBatchRendererAsDefault = true;
                        break;
                    case GLFW_KEY_MINUS:
                        GJGO::Renderer::useBatchRendererAsDefault = false;
                        break;
                }
                break;
            case GJGO::EventType::keyUp:
                GJGO_LOG_INFO("UP  : ", a_event.keycode);
                break;
            case GJGO::EventType::keyTypedDown:
                //GJGO_LOG_INFO(a_event.keycode);
                break;
            case GJGO::EventType::mouseMove:
                //GJGO_LOG_INFO("POS: ", '(', a_event.mousePosition.relative.x, ", ", a_event.mousePosition.relative.y, ')');
                break;
            case GJGO::EventType::mouseButtonDown:
                //GJGO_LOG_INFO("DOWN: ", static_cast<unsigned short>(a_event.mouseButton));
                break;
            case GJGO::EventType::mouseButtonUp:
                //GJGO_LOG_INFO("UP  : ", static_cast<unsigned short>(a_event.mouseButton));
                break;
            case GJGO::EventType::mouseWheelScroll:
                GJGO_LOG_INFO("Wheel Dir: ", static_cast<short>(a_event.mouseWheelDirection));
                break;
            case GJGO::EventType::windowResize:
                //GJGO_LOG_INFO("Win Size: ", '(', a_event.windowSize.x, ", ", a_event.windowSize.y, ')');
                break;
        }
    }
};

int main()
{
    GJGO::AppSettings settings;

    GJGO::App app(settings);

    app.layers.emplace_back(new GameLayer);

    app.run();

    return 0;
}
