workspace("GJGO")
    configurations({"Debug", "Release"})
    platforms({"LinuxARM"})

    project("GJGO")
        filename("GJGO")
        kind("StaticLib")
        language("C++")

        files({"premake5.lua", ".gitignore", "src/*.cpp", "include/*"})

        filter("platforms:LinuxARM")
            architecture("ARM")
            system("linux")

        filter("configurations:Debug")
            optimize("Off")
            symbols("On")
            linkoptions({"-fsanitize=address", "-fsanitize=leak", "-static-libasan"})

        filter("configurations:Release")
            optimize("On")
            flags({"LinkTimeOptimization"})
