#include <GJGOpch.hpp>

#include <GJGO/csharp_api.hpp>
#include <GJGO/profiler.hpp>

namespace GJGO
{
    namespace CSharpAPI
    {
        static inline MonoAssembly* apiAssembly;

        void init()
        {
            GJGO_PROFILE_FUNCTION();

            mono_config_parse(nullptr);

            domain = mono_jit_init("GJGO");

            apiAssembly = mono_domain_assembly_open(domain, "GJGO.dll");
            if (!apiAssembly)
            {
                std::cout << "ERROR: Invalid assembly!" << std::endl;
                exit(1);
            }

            MonoImage* image = mono_assembly_get_image(apiAssembly);

            MonoMethodDesc* methodDesc = mono_method_desc_new("GJGO.Entity:Test", true);
            if (!methodDesc)
            {
                std::cout << "ERROR: Invalid method description!" << std::endl;
                exit(1);
            }

            MonoMethod* method = mono_method_desc_search_in_image(methodDesc, image);
            if (!method)
            {
                std::cout << "ERROR: Invalid method!" << std::endl;
                exit(1);
            }

            mono_runtime_invoke(method, nullptr, nullptr, nullptr);
        }

        void shutdown()
        {
            GJGO_PROFILE_FUNCTION();

            mono_jit_cleanup(domain);
        }
    }
}
