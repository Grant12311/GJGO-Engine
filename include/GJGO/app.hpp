#ifndef GJGO_APP_H
#define GJGO_APP_H

namespace GJGO
{
    class App
    {
    public:
        App();
        ~App();

        inline static App* instance;
    };
}

#endif
