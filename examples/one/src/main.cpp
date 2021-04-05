#include <GJGO/gjgo.hpp>

class GameLayer : public GJGO::Layer
{
public:
    GJGO::Camera2D cam;
    glm::vec2 pos = glm::vec2(100.0f, 100.0f);
    glm::vec2 size = glm::vec2(100.0f, 100.0f);
    float rotation = 0.0f;

    GameLayer()
    {
        this->name = "Game Layer";

        GJGO::Texture2D::create("res/wall.jpg", false, GL_NEAREST, GL_NEAREST);
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

            GJGO::Window::setTitle(std::to_string(1.0 / (total / times.size()) * 1000.0).c_str());

            timePassed = 0.0;
            times.clear();
        }else{
            times.emplace_back(GJGO::App::instance->deltaTime);
            timePassed += GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_W))
        {
            this->pos.y += 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_S))
        {
            this->pos.y -= 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_A))
        {
            this->pos.x -= 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_D))
        {
            this->pos.x += 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_UP))
        {
            this->size += 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_DOWN))
        {
            this->size -= 1.0f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_RIGHT))
        {
            this->rotation += 0.1f * GJGO::App::instance->deltaTime;
        }

        if (GJGO::Keyboard::keyIsDown(GLFW_KEY_LEFT))
        {
            this->rotation -= 0.1f * GJGO::App::instance->deltaTime;
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

    virtual void draw() override
    {
        GJGO::Renderer::begin2D(cam, GJGO::Window::getWidth(), GJGO::Window::getHeight());

        GJGO::Renderer::drawQuad(this->pos, this->size, this->rotation, {1.0f, 1.0f, 1.0f, 1.0f}, GJGO::Texture2D::get("res/wall.jpg"));
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
