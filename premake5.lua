workspace "GJGO"
    language "C++"
    cppdialect "C++17"
    configurations {"Debug", "Release", "Dist"}
    platforms {"LinuxX64", "LinuxX86", "LinuxARM", "WindowsARM", "WindowsX86", "WindowsX64"}
    startproject "GJGO"

    flags {"MultiProcessorCompile"}
    buildoptions {"-pipe", "-Wall"}
    defines {}

    links {"glfw", "GL", "X11", "dl"}
    includedirs {"%{wks.location}/include/", "%{wks.location}/vendor/glm/", "%{wks.location}/vendor/", "%{wks.location}/vendor/glad/include/", "%{wks.location}/vendor/entt/src/", "%{wks.location}/vendor/Druid/include/"}

    filter "action:vs*"
        toolset "msc"

    filter "action:not vs*"
        toolset "gcc"

    filter "files:not glad.c"
        buildoptions {"-Wpedantic", "-Wextra", "-Wnon-virtual-dtor", "-Winline", "-Wunreachable-code", "-Wshadow", "-Wconversion", "-Wno-switch", "-Wno-unused-variable", "-Wold-style-cast",
                      "-Wcast-align", "-Wunused", "-Woverloaded-virtual", "-Wsign-conversion", "-Wduplicated-cond", "-Wduplicated-branches", "-Wlogical-op", "-Wnull-dereference", "-Wuseless-cast",
                      "-Wdouble-promotion"}

    filter "platforms:Linux*"
        system "linux"
        defines {"GJGO_PLATFORM_LINUX"}
    filter "platforms:Windows*"
        system "windows"
        defines {"GJGO_PLATFORM_WINDOWS"}

    filter "platforms:*ARM"
        architecture "ARM"
    filter "platforms:*X86"
        architecture "x86"
    filter "platforms:*X64"
        architecture "x86_64"

    filter "configurations:Debug"
        symbols "On"
        links {"ubsan"}
        buildoptions {"-pg", "-fsanitize=address", "-fsanitize=leak", "-fsanitize=undefined", "-static-libasan"}
        linkoptions {"-pg", "-fsanitize=address", "-fsanitize=leak", "-fsanitize=undefined", "-static-libasan"}
        defines {"GJGO_BUILD_CONFIG_DEBUG"}

    filter "configurations:Release"
        optimize "Speed"
        flags {"LinkTimeOptimization"}
        defines {"GJGO_BUILD_CONFIG_RELEASE"}

    filter "configurations:Dist"
        optimize "Speed"
        flags {"LinkTimeOptimization"}
        defines {"GJGO_BUILD_CONFIG_DIST"}

    include "vendor/Druid/project.lua"

    project "GJGO"
        kind "StaticLib"
        location "%{wks.location}"
        targetdir "%{prj.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

        files {"%{prj.location}/src/**.cpp", "%{prj.location}/include/**.hpp", "%{prj.location}/include/**.h", "%{prj.location}/vendor/glad/src/glad.c", "%{prj.location}/premake5.lua"}

    project "One"
        location "%{wks.location}/examples/one/"
        targetdir "%{prj.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

        links {"GJGO", "Druid", "glfw", "GL", "X11", "dl"}

        filter "configurations:Debug or Release"
            kind "ConsoleApp"

        filter "configurations:Dist"
            kind "WindowedApp"

        files {"%{prj.location}/src/**.cpp", "%{prj.location}/include/**.hpp", "%{prj.location}/include/**.h"}
