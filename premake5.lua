workspace "GJGO"
    language "C++"
    cppdialect "C++17"
    configurations {"Debug", "Release", "Dist"}
    platforms {"LinuxX64", "LinuxX86", "LinuxARM", "WindowsARM", "WindowsX86", "WindowsX64"}
    startproject "GJGO"

    flags {"MultiProcessorCompile"}
    buildoptions {"-pipe", "-Wall"}
    defines {}

    links {}
    includedirs {"%{wks.location}/include/"}

    filter "action:vs*"
        toolset "msc"

    filter "action:not vs*"
        toolset "gcc"
        buildoptions {"-Wextra", "-Wnon-virtual-dtor", "-Winline", "-Wunreachable-code", "-Wshadow", "-Wconversion", "-Wno-switch", "-Wno-unused-variable"}

    filter "platforms:Linux*"
        system "linux"
    filter "platforms:Windows*"
        system "windows"

    filter "platforms:*ARM"
        architecture "ARM"
    filter "platforms:*X86"
        architecture "x86"
    filter "platforms:*X64"
        architecture "x86_64"

    filter "configurations:Debug"
        symbols "On"
        buildoptions {"-pg", "-fsanitize=address", "-fsanitize=leak", "-static-libasan"}
        linkoptions {"-pg", "-fsanitize=address", "-fsanitize=leak", "-static-libasan"}

    filter "configurations:Release"
        optimize "On"

    project "GJGO"
        kind "ConsoleApp"
        location "%{wks.location}"
        targetdir "%{prj.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

        files {"%{prj.location}/src/**.cpp", "%{prj.location}/include/**.hpp", "%{prj.location}/include/**.h", "%{prj.location}/premake5.lua"}


