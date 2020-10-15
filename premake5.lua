workspace("GJGO")
    toolset("gcc")
    configurations({"Debug", "Release"})
    platforms({"LinuxARM"})

    project("GJGO")
        filename("GJGO")
        kind("StaticLib")
        language("C++")

        buildoptions({"-std=c++1z", "-Wall", "-Wnon-virtual-dtor", "-Winline", "-Wunreachable-code"})
        files({"premake5.lua", ".gitignore", "src/*.cpp", "include/*"})

        filter("platforms:LinuxARM")
            architecture("ARM")
            system("linux")

        filter("configurations:Debug")
            symbols("On")
            buildoptions({"-pg"})
            linkoptions({"-pg", "-fsanitize=address", "-fsanitize=leak", "-static-libasan"})

        filter("configurations:Release")
            optimize("On")
            flags({"LinkTimeOptimization"})
