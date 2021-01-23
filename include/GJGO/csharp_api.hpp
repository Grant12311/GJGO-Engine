#ifndef GJGO_CSHARP_API_H
#define GJGO_CSHARP_API_H

#include <mono/jit/jit.h>

namespace GJGO
{
    namespace CSharpAPI
    {
        inline MonoDomain* domain;

        void init();
        void shutdown();
    }
}

#endif
