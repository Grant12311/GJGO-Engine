workspace("GJGO")
    language("C++")
    cppdialect("C++17")
    toolset("gcc")
    configurations({"Debug", "Release", "Dist"})
    platforms({"LinuxARM"})
    startproject("Sprite Test")

    includedirs({"include", "vendor/Druid/include", "vendor/Beacon/include", "vendor/entt/src", "vendor/imgui", "vendor/clip", "vendor/glm"})
    links({"X11", "xcb", "xcb-randr", "EGL", "GL", "pthread", "glfw", "dl"})

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
        kind("StaticLib")
        links({"Druid", "imgui", "clip"})

        files({"premake5.lua", ".gitignore", "src/**", "include/**"})

    project("Sprite Test")
        location("examples/Sprite Test")
        targetdir("examples/Sprite Test/bin/%{cfg.platform}/%{cfg.buildcfg}")

        links({"GJGO", "Druid", "imgui", "clip"})
        files({"examples/Sprite Test/src/*"})

        filter("configurations:Debug")
            kind("ConsoleApp")

        filter("configurations:Release")
            kind("ConsoleApp")

        filter("configurations:Dist")
            kind("WindowedApp")


