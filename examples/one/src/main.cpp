#include <algorithm>
#include <array>
#include <string>
#include <vector>

#include <tweeny/tweeny.h>

#include <GJGO/gjgo.hpp>

constexpr int GBA_SCREEN_WDITH = 240;
constexpr int GBA_SCREEN_HEIGHT = 160;
constexpr int GBA_SCALE = 3;

constexpr int TILE_SIZE = 16  * GBA_SCALE;

class GameLayer final : public GJGO::Layer
{
public:
    GJGO::Entity fadeScreen;
    bool runFade;
    tweeny::tween<double> fadeTween = tweeny::from(0.0).to(1.0).during(300).to(0.0).during(750).to(0.0).during(300);

    GJGO::Entity player;

    GameLayer() :
        Layer("Game Layer"), fadeScreen(GJGO::Entity::create()), runFade(false), player(GJGO::Entity::create())
    {
        glfwSwapInterval(1);

        this->fadeScreen.addComponent<GJGO::Transform2DComponent>(glm::vec3(-100.0, -100.0, 50.0), glm::vec2(GBA_SCREEN_WDITH * GBA_SCALE * 3, GBA_SCREEN_HEIGHT * GBA_SCALE * 3));
        this->fadeScreen.addComponent<GJGO::SpriteComponent>(nullptr, glm::vec4(0.0, 0.0, 0.0, 0.0));

        this->player.addComponent<GJGO::Transform2DComponent>(glm::vec3(9 * TILE_SIZE, 4 * TILE_SIZE, 1.0), glm::vec2(14 * GBA_SCALE, 21 * GBA_SCALE));
        this->player.addComponent<GJGO::SpriteComponent>(GJGO::Texture2D::create("res/Player/Male/standing south.png", GJGO::TextureSettings::standard, GL_NEAREST, GL_NEAREST));
        this->player.addComponent<GJGO::CollisionBox2DComponent>(glm::vec2(0.0, 0.0), glm::vec2(1.142857143, 0.761904762));

        GJGO::Entity doorEntity = GJGO::Entity::create();
        doorEntity.addComponent<GJGO::Transform2DComponent>(glm::vec3(9 * TILE_SIZE, 5 * TILE_SIZE, 0.0), glm::vec2(14 * GBA_SCALE, 21 * GBA_SCALE));
        doorEntity.addComponent<GJGO::CollisionBox2DComponent>(glm::vec2(0, 0), glm::vec2(1.0, 1.0),
        std::function<void(GJGO::Entity, GJGO::Entity)>(),
        [this](const GJGO::Entity /*l_entity1*/, const GJGO::Entity l_entity2) -> void
        {
            this->fadeScreen.getComponent<GJGO::Transform2DComponent>().position = glm::vec3(GJGO::Camera2D::primary->position.x, GJGO::Camera2D::primary->position.y, 50.0);
            this->runFade = true;
        });

        GJGO::Entity outsideBackgroundEntity = GJGO::Entity::create();
        outsideBackgroundEntity.addComponent<GJGO::Transform2DComponent>(glm::vec3(0.0, 0.0, 0.0), glm::vec2(383 * GBA_SCALE, 359 * GBA_SCALE));
        outsideBackgroundEntity.addComponent<GJGO::SpriteComponent>(GJGO::Texture2D::create("res/Backgrounds/Littleroot Town/outside.png", GJGO::TextureSettings::standard, GL_NEAREST, GL_NEAREST));

        GJGO::Entity labBackgroundEntity = GJGO::Entity::create();
        labBackgroundEntity.addComponent<GJGO::Transform2DComponent>(glm::vec3(-300.0 * GBA_SCALE, -300.0 * GBA_SCALE, 0.0), glm::vec2(208 * GBA_SCALE, 208 * GBA_SCALE));
        labBackgroundEntity.addComponent<GJGO::SpriteComponent>(GJGO::Texture2D::create("res/Backgrounds/Littleroot Town/lab.png", GJGO::TextureSettings::standard, GL_NEAREST, GL_NEAREST));
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

        if (this->runFade)
        {
            this->fadeScreen.getComponent<GJGO::SpriteComponent>().color.a = this->fadeTween.step(static_cast<unsigned int>(GJGO::App::instance->deltaTime));
            if (this->fadeTween.progress() >= 1.0f)
            {
                this->runFade = false;
                this->fadeTween.seek(0.0f);
            }
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
                    case GLFW_KEY_W:
                        this->player.getComponent<GJGO::Transform2DComponent>().position.y += TILE_SIZE;
                        if ((this->player.getComponent<GJGO::Transform2DComponent>().position.y - GJGO::Camera2D::primary->position.y) / TILE_SIZE >= 6)
                            GJGO::Camera2D::primary->position.y += TILE_SIZE;
                        break;
                    case GLFW_KEY_A:
                        this->player.getComponent<GJGO::Transform2DComponent>().position.x -= TILE_SIZE;
                        if ((this->player.getComponent<GJGO::Transform2DComponent>().position.x - GJGO::Camera2D::primary->position.x) / TILE_SIZE <= 5)
                            GJGO::Camera2D::primary->position.x -= TILE_SIZE;
                        break;
                    case GLFW_KEY_S:
                        this->player.getComponent<GJGO::Transform2DComponent>().position.y -= TILE_SIZE;
                        if ((this->player.getComponent<GJGO::Transform2DComponent>().position.y - GJGO::Camera2D::primary->position.y) / TILE_SIZE <= 3)
                            GJGO::Camera2D::primary->position.y -= TILE_SIZE;
                        break;
                    case GLFW_KEY_D:
                        this->player.getComponent<GJGO::Transform2DComponent>().position.x += TILE_SIZE;
                        if ((this->player.getComponent<GJGO::Transform2DComponent>().position.x - GJGO::Camera2D::primary->position.x) / TILE_SIZE >= 10)
                            GJGO::Camera2D::primary->position.x += TILE_SIZE;
                        break;
                }
                break;
            default:
                break;
        }
    }
};

int main()
{
    GJGO::App app(GJGO::AppSettings("PKMN", GBA_SCREEN_WDITH * GBA_SCALE, GBA_SCREEN_HEIGHT * GBA_SCALE, false));
    app.layers.emplace_back(new GameLayer);
    app.run();

    return 0;
}
