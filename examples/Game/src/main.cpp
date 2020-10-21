#include <iostream>

#include <GJGO/app.hpp>
#include <GJGO/log.hpp>

int main()
{
    GJGO_SET_LOG_LEVEL_WARN();
    GJGO::Application app;
    app.run();

    return 0;
}
