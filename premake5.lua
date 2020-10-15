workspace("GJGO")
    configurations({"Debug", "Release"})
    platforms({"Linux"})

    project("GJGO")
        filename("GJGO")
        kind("StaticLib")
        language("C++")

        files({"premake5.lua", ".gitignore", "src/*.cpp", "include/*"})
