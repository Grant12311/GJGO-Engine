workspace("GJGO")
    language("C++")
    toolset("gcc")
    configurations({"Debug", "Release"})
    platforms({"LinuxARM"})

    includedirs({"include", "vendor/Hangar2/include", "vendor/Druid/include"})
    links({"X11", "GL"})

    buildoptions({"-std=c++1z", "-Wall", "-Wnon-virtual-dtor", "-Winline", "-Wunreachable-code"})

    filter("platforms:LinuxARM")
        architecture("ARM")
        system("linux")

        libdirs({"/usr/lib"})

    filter("configurations:Debug")
        symbols("On")
        buildoptions({{"-pg", "-fsanitize=address", "-fsanitize=leak", "-static-libasan"}})
        linkoptions({"-pg", "-fsanitize=address", "-fsanitize=leak", "-static-libasan"})
        defines({"GJGO_BUILD_TARGET_DEBUG"})

    filter("configurations:Release")
        optimize("On")
        flags({"LinkTimeOptimization"})
        defines({"GJGO_BUILD_TARGET_RELEASE"})

    project("Druid")
        filename("Druid")
        kind("StaticLib")
        location("vendor/Druid")
        targetdir("vendor/Druid/bin/%{cfg.platform}/%{cfg.buildcfg}")

        files({"vendor/Druid/src/**", "vendor/Druid/include/**"})

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
            kind("WindowedApp")


