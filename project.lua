project("GJGO")
    language("C++")
    cppdialect("C++17")
    toolset("gcc")
    configurations({"Debug", "Release", "Dist"})
    platforms({"LinuxARM"})
    kind("StaticLib")

    includedirs({"include", "vendor/Druid/include", "vendor/Beacon/include", "vendor/entt/src", "vendor/imgui", "vendor/clip", "vendor/glm", "vendor/tweeny/include/"})

    flags({"MultiProcessorCompile"})
    buildoptions({"-pipe", "-Wall", "-Wextra", "-Wnon-virtual-dtor", "-Winline", "-Wunreachable-code", "-Wshadow", "-Wconversion", "-Wno-switch", "-Wno-unused-variable"})
    defines({"IMGUI_IMPL_OPENGL_ES3", "GLFW_INCLUDE_ES31", "GLFW_INCLUDE_GLEXT"})

    files({"premake5.lua", "project.lua", ".gitignore", "src/**", "include/**"})

    pchheader("pch.hpp")

    filter("platforms:Linux*")
        system("linux")

    filter("platforms:*ARM")
        architecture("ARM")

    filter("platforms:*i386")
        architecture("x86")

    filter("configurations:Debug")
        symbols("On")
        buildoptions({{"-pg", "-fsanitize=address", "-fsanitize=leak", "-static-libasan"}})
        linkoptions({"-pg", "-fsanitize=address", "-fsanitize=leak", "-static-libasan"})
        defines({"GJGO_BUILD_TARGET_DEBUG"})

    filter("configurations:Release")
        optimize("On")
        defines({"DRUID_DISABLE_GLCALL"})
        defines({"GJGO_BUILD_TARGET_RELEASE"})

    filter("configurations:Dist")
        optimize("On")
        defines({"DRUID_DISABLE_GLCALL"})
        defines({"GJGO_BUILD_TARGET_DIST"})
