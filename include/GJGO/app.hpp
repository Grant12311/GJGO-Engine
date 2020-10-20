#include <Hangar2/hangar.h>

namespace GJGO
{
    class Application
    {
    public:
        Hangar::Window window;

        void run();

        Application(const Hangar::Config &a_config = Hangar::Config());
    };
}
