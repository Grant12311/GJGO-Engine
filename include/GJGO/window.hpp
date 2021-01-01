#ifndef GJGO_WINDOW_H
#define GJGO_WINDOW_H

namespace GJGO
{
    namespace Window
    {
        inline double deltaTime = 0;

        void setTitle(const char* const a_title);

        unsigned int getWidth();
        unsigned int getHeight();
    }
}

#endif
