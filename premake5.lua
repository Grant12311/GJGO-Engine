workspace("GJGO")
    language("C++")
    toolset("gcc")
    configurations({"Debug", "Release"})
    platforms({"LinuxARM"})

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

    filter("configurations:Release")
        optimize("On")
        flags({"LinkTimeOptimization"})

    project("Druid")
        filename("Druid")
        kind("StaticLib")

        files({"vendor/Druid/src/*", "vendor/Druid/include/*"})

    project("GJGO")
        filename("GJGO")
        kind("StaticLib")
        links({"Druid"})

        files({"premake5.lua", ".gitignore", "src/*", "include/*"})

    project("Game")
        filename("Game")

        links({"GJGO", "Druid"})
        files({"game/src/*"})

        filter("configurations:Debug")
            kind("ConsoleApp")

        filter("configurations:Release")
            kind("WindowedApp")


