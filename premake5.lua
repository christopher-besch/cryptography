outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "cryptography"
    startproject "decryption"

    configurations { "Debug", "Release", "Dist" }

    -- when "IDE" defined, program reads form cwd instead of executable location
    filter "configurations:Debug"
        defines { "DEBUG", "IDE" }
        symbols "On"
    filter "configurations:Release"
        defines { "RELEASE", "IDE" }
        symbols "On"
        optimize "On"
    filter "configurations:Dist"
        defines { "DIST" }
        optimize "On"
    
    filter "system:linux"
        defines { "LINUX" }
    filter "system:windows"
        defines { "WINDOWS" }

    -- cwd
    debugdir ""

    -- include mingw libraries statically
    configuration { "windows", "gmake2" }
        buildoptions { "-static", "-static-libgcc", "-static-libstdc++" }

project "decryption"
    language "C++"
    architecture "x86_64"
    cppdialect "C++17"
    kind "ConsoleApp"

    location "%{prj.name}"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "utils/src"
    }

    links {
        "utils"
    }

project "encryption"
    language "C++"
    architecture "x86_64"
    cppdialect "C++17"
    kind "ConsoleApp"

    location "%{prj.name}"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "utils/src"
    }

    links {
        "utils"
    }

-- dependencies
include "utils"
