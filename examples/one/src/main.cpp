#include <GJGO/gjgo.hpp>

class GameLayer : public GJGO::Layer
{
public:
    GameLayer()
    {
        this->name = "Game Layer";
    }

    virtual void onUpdate() override
    {
        static std::vector<double> times;

        if (times.size() == 100)
            times.erase(times.begin());

        times.emplace_back(GJGO::App::instance->deltaTime);

        double total;
        for (const double l_dt : times)
        {
            total += l_dt;
        }

        GJGO_LOG_INFO("dt: ", 1.0 / (total / times.size()) * 1000.0);
    }

    virtual void onEvent(const GJGO::Event &a_event) override
    {
        switch (a_event.type)
        {
            case GJGO::EventType::keyDown:
                GJGO_LOG_INFO("DOWN: ", a_event.keycode);
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
