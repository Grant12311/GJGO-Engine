workspace("GJGO")
    language("C++")
    cppdialect("C++17")
    toolset("gcc")
    configurations({"Debug", "Release", "Dist"})
    platforms({"LinuxARM"})
    startproject("Game")

    includedirs({"include", "vendor/Druid/include", "vendor/Beacon/include", "vendor/entt/src", "vendor/imgui", "vendor/clip", "vendor/glm"})
    links({"X11", "xcb", "xcb-randr", "GL", "pthread", "glfw3", "dl"})

    flags({"MultiProcessorCompile"})
    buildoptions({"-pipe", "-Wall", "-Wextra", "-Wnon-virtual-dtor", "-Winline", "-Wunreachable-code", "-Wshadow", "-Wconversion", "-Wno-switch", "-Wno-unused-variable"})
    defines({"IMGUI_IMPL_OPENGL_ES3", "GLFW_INCLUDE_ES31", "GLFW_INCLUDE_GLEXT"})

    filter("platforms:LinuxARM")
        architecture("ARM")
        system("linux")

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

    include("vendor/imgui/project.lua")
    include("vendor/clip/project.lua")
    include("vendor/Druid/project.lua")

    project("GJGO")
        filename("GJGO")
        kind("StaticLib")
        links({"Druid", "imgui", "clip"})

        files({"premake5.lua", ".gitignore", "src/**", "include/**"})

    project("Game")
        filename("Game")
        location("examples/Game")
        targetdir("examples/Game/bin/%{cfg.platform}/%{cfg.buildcfg}")

        links({"GJGO", "Druid", "imgui", "clip"})
        files({"examples/Game/src/*"})

        filter("configurations:Debug")
            kind("ConsoleApp")

        filter("configurations:Release")
            kind("ConsoleApp")

        filter("configurations:Dist")
            kind("WindowedApp")


