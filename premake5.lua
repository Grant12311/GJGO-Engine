workspace "GJGO"
    language "C++"
    cppdialect "C++17"
    configurations {"Debug", "Release", "Dist"}
    platforms {"LinuxX64", "LinuxX86", "LinuxARM", "WindowsARM", "WindowsX86", "WindowsX64"}
    startproject "GJGO"

    flags {"MultiProcessorCompile"}
    buildoptions {"-pipe", "-Wall"}
    defines {}

    links {"glfw", "GL", "X11"}
    includedirs {"%{wks.location}/include/", "%{wks.location}/vendor/glm/", "%{wks.location}/vendor/glad/include/"}

    filter "action:vs*"
        toolset "msc"

    filter "action:not vs*"
        toolset "gcc"
        buildoptions {"-Wextra", "-Wnon-virtual-dtor", "-Winline", "-Wunreachable-code", "-Wshadow", "-Wconversion", "-Wno-switch", "-Wno-unused-variable"}

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
        buildoptions {"-pg", "-fsanitize=address", "-fsanitize=leak", "-static-libasan"}
        linkoptions {"-pg", "-fsanitize=address", "-fsanitize=leak", "-static-libasan"}
        defines {"GJGO_BUILD_CONFIG_DEBUG"}

    filter "configurations:Release"
        optimize "On"
        defines {"GJGO_BUILD_CONFIG_RELEASE"}

    filter "configurations:Dist"
        optimize "On"
        defines {"GJGO_BUILD_CONFIG_DIST"}

    project "GJGO"
        kind "StaticLib"
        location "%{wks.location}"
        targetdir "%{prj.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

        files {"%{prj.location}/src/**.cpp", "%{prj.location}/include/**.hpp", "%{prj.location}/include/**.h", "%{prj.location}/vendor/glad/src/glad.c", "%{prj.location}/premake5.lua"}

    project "One"
        location "%{wks.location}/examples/one/"
        targetdir "%{prj.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

        links {"GJGO", "glfw", "GL", "X11"}

        filter "configurations:Debug or Release"
            kind "ConsoleApp"

        filter "configurations:Dist"
            kind "WindowedApp"

        files {"%{prj.location}/src/**.cpp", "%{prj.location}/include/**.hpp", "%{prj.location}/include/**.h"}
