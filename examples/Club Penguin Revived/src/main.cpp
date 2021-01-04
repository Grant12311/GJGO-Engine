#include <pch.hpp>

#include <nlohmann/json.hpp>

#include <Druid/coordinates.h>

#include <GJGO/2D/renderer2D.hpp>
#include <GJGO/animation.hpp>
#include <GJGO/app.hpp>
#include <GJGO/2D/camera2D.hpp>
#include <GJGO/layer.hpp>
#include <GJGO/log.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/window.hpp>

#include <CPRV/door.hpp>
#include <CPRV/loaded_texture_details.hpp>
#include <CPRV/load_texture_thread_fn.hpp>

using json = nlohmann::json;

constexpr static unsigned int getDistance(const GJGO::Position2D &a_point1, const GJGO::Position2D &a_point2)
{
    return std::sqrt(std::pow(a_point2.x - a_point1.x, 2) + std::pow(a_point2.y - a_point1.y, 2));
}

class GameLayer : public GJGO::Layer
{
public:
    json roomData;

    GJGO::Camera2D camera;
    GJGO::Position2D playerPosition;
    GJGO::AnimationPosition2D* animationPtr = nullptr;
    std::vector<Door> doors;

    Druid::Shader shader;
    Druid::Texture2D playerTexture;
    std::future<LoadedTextureDetails> loadingTexture;
    bool loadingNextRoom;

    Druid::Texture2D loadScreenBackgroundTexture;
    Druid::Texture2D loadingScreenBarTexture;
    Druid::Texture2D loadingScreenWheelTexture;

    std::string currentRoomName = "Town";
    Druid::Texture2D* currentRoomTexturePtr;

    void loadRoom(const std::string &a_name)
    {
        this->loadingNextRoom = true;

        this->loadingTexture = std::async(std::launch::async, loadTextureThreadFn, roomData[a_name]["file"]);

        this->doors.clear();
        if (this->roomData[a_name].contains("doors"))
        {
            for (auto& l_element : this->roomData[a_name]["doors"])
            {
                GJGO::Position2D pos = {l_element["position"][0], l_element["position"][1]};
                GJGO::Size2D size = {l_element["size"][0], l_element["size"][1]};
                this->doors.emplace_back(pos, size, l_element["target"]);
            }
        }

        this->playerPosition = {0, 0};

        this->currentRoomName = a_name;
    }

    void onUpdate() override
    {
        GJGO_PROFILE_FUNCTION();

        if (this->loadingNextRoom)
        {
            std::future_status status = loadingTexture.wait_for(std::chrono::nanoseconds(1));
            if (status == std::future_status::ready)
            {
                LoadedTextureDetails details = this->loadingTexture.get();
                this->currentRoomTexturePtr = new Druid::Texture2D(details.data, details.width, details.height, details.bpp, GL_LINEAR, GL_LINEAR);
                stbi_image_free(details.data);
                this->loadingNextRoom = false;
            }
        }

        if (!this->loadingNextRoom)
        {
            if (this->animationPtr != nullptr)
            {
                this->animationPtr->step();
                if (this->animationPtr->isDone())
                {
                    delete this->animationPtr;
                    this->animationPtr = nullptr;
                }
            }else{
                unsigned short i = 0;
                for (const Door &l_door : this->doors)
                {
                    if (Druid::coordsInRect(l_door.position.x, l_door.position.y, l_door.size.height, l_door.size.width, this->playerPosition.x, this->playerPosition.y))
                    {
                        this->loadRoom(l_door.target);
                    }
                    i++;
                }
            }
        }
    }

    void onEvent(GJGO::Event* const a_event) override
    {
        GJGO_PROFILE_FUNCTION();

        if (!this->loadingNextRoom)
        {
            switch (a_event->type)
            {
                case GJGO::EventType::keyDown:
                    switch (a_event->keycode)
                    {
                        case GLFW_KEY_0:
                            GJGO::setVsync(false);
                            break;
                        case GLFW_KEY_1:
                            GJGO::setVsync(true);
                            GJGO::setFramerateCap(60);
                            break;
                        case GLFW_KEY_2:
                            GJGO::setVsync(true);
                            GJGO::setFramerateCap(30);
                            break;
                    }
                    break;
                case GJGO::EventType::mouseButtonDown:
                {
                    std::array<double, 2> mousePosition;
                    glfwGetCursorPos(GJGO::g_appInstancePtr->windowPtr, &mousePosition[0], &mousePosition[1]);
                    mousePosition[1] = GJGO::Window::getHeight() - mousePosition[1];
                    std::cout << "(" << mousePosition[0] << ", " << mousePosition[1] << ")" << std::endl;

                    if (this->animationPtr != nullptr)
                    {
                        delete this->animationPtr;
                    }

                    this->animationPtr = new GJGO::AnimationPosition2D(getDistance(this->playerPosition, GJGO::Position2D{static_cast<int>(mousePosition[0]), static_cast<int>(mousePosition[1])}),
                                                                this->playerPosition, GJGO::Position2D{static_cast<int>(mousePosition[0]), static_cast<int>(mousePosition[1])});

                    break;
                }
            }
        }
    }

    void draw() override
    {
        GJGO_PROFILE_FUNCTION();

        GJGO::Renderer::begin2D(&this->shader, this->camera, GJGO::Window::getWidth(), GJGO::Window::getHeight());

        if (this->loadingNextRoom)
        {
            static float wheelRotation = 0.0f;
            GJGO::Renderer::drawQuad({0, 0}, {1235, 780}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, this->loadScreenBackgroundTexture);
            GJGO::Renderer::drawQuad({545, 375}, {145, 31}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, this->loadingScreenBarTexture);
            GJGO::Renderer::drawQuad({551, 383}, {21, 17}, wheelRotation, {1.0f, 1.0f, 1.0f, 1.0f}, this->loadingScreenWheelTexture);

            wheelRotation += GJGO::Window::deltaTime / 2.0f;

            if (wheelRotation >= 360.0f)
                wheelRotation -= 360.0f;
        }else{
            GJGO::Renderer::drawQuad({0, 0}, {1235, 780}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, *this->currentRoomTexturePtr);

            for (const Door &l_door : this->doors)
            {
                GJGO::Renderer::drawQuad(l_door.position, l_door.size, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f});
            }

            constexpr int playerPosModX = -30;
            constexpr int playerPosModY = -10;
            GJGO::Renderer::drawQuad({this->playerPosition.x + playerPosModX, this->playerPosition.y + playerPosModY}, {61, 69}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, this->playerTexture);
        }
    }

    void drawGui() override
    {
        GJGO_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();

        ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs;
        ImGui::SetNextWindowPos({static_cast<float>(GJGO::Window::getWidth()) - ImGui::CalcTextSize("16.667 ms/frame (60.0 FPS)").x - 10.0f, 10.0f});
        if (ImGui::Begin("Example: Simple overlay", NULL, overlayFlags))
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }
        ImGui::End();
    }

    GameLayer() :
        shader("sprite.shader"), playerTexture("res/penguin/purple/down.png", false, GL_NEAREST, GL_NEAREST),
        loadScreenBackgroundTexture("res/loading_screen/background.png", false, GL_LINEAR, GL_LINEAR),
        loadingScreenBarTexture("res/loading_screen/loading_bar_no_wheel.png", false, GL_LINEAR, GL_LINEAR),
        loadingScreenWheelTexture("res/loading_screen/loading_wheel.png", false, GL_LINEAR, GL_LINEAR)
    {
        GJGO_PROFILE_FUNCTION();

        this->name = "Game";

        this->doors.reserve(10);

        std::ifstream jsonFile("room data.json");
        jsonFile >> this->roomData;
        jsonFile.close();

        this->loadRoom("Town");
    }

    ~GameLayer()
    {
        delete this->currentRoomTexturePtr;
    }
};

int main()
{
    GJGO_LOG_SET_PRINT_FILE(false);
    GJGO_LOG_SET_PRINT_FUNCTION(false);
    GJGO::Application app;

    GJGO::Window::setTitle("Club Penguin Revived");
    GJGO::Window::setSize(1235, 780);

    app.layers.emplace_back(new GameLayer);

    app.run();

    return 0;
}
