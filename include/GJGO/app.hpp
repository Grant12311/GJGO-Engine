#ifndef GJGO_APP_H
#define GJGO_APP_H

namespace GJGO
{
    class App
    {
    public:
        App();
        ~App();

        void run();

        inline static App* instance;
    };
}

#endif
