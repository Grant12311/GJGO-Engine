workspace("GJGO")
    language("C++")
    cppdialect("C++17")
    toolset("gcc")
    configurations({"Debug", "Release", "Dist"})
    platforms({"LinuxARM"})

    includedirs({"include", "vendor/Hangar2/include", "vendor/Druid/include", "vendor/Beacon/include", "vendor/entt/src"})
    links({"X11", "GL"})

    flags({"MultiProcessorCompile"})
    buildoptions({"-pipe", "-Wall", "-Wextra", "-Wnon-virtual-dtor", "-Winline", "-Wunreachable-code", "-Wshadow"})

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
        flags({"LinkTimeOptimization"})
        defines({"DRUID_DISABLE_GLCALL"})
        defines({"GJGO_BUILD_TARGET_RELEASE"})

    filter("configurations:Dist")
        optimize("On")
        flags({"LinkTimeOptimization"})
        defines({"DRUID_DISABLE_GLCALL"})
        defines({"GJGO_BUILD_TARGET_DIST"})

    include("vendor/Druid/project.lua")

    project("GJGO")
        filename("GJGO")
        kind("StaticLib")
        links({"Druid"})

        files({"premake5.lua", ".gitignore", "src/*", "include/**"})

    project("Game")
        filename("Game")
        location("examples/Game")
        targetdir("examples/Game/bin/%{cfg.platform}/%{cfg.buildcfg}")

        links({"GJGO", "Druid"})
        files({"examples/Game/src/*"})

        filter("configurations:Debug")
            kind("ConsoleApp")

        filter("configurations:Release")
            kind("ConsoleApp")

        filter("configurations:Dist")
            kind("WindowedApp")


