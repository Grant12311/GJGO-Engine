#include <GJGO/app.hpp>

namespace GJGO
{
    App::App()
    {
        App::instance = this;
    }

    App::~App()
    {

    }
}
