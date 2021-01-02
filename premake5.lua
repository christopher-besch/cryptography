outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "cryptography"
    startproject "decryption"

    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"
    -- cwd
    debugdir ""

project "decryption"
    language "C++"
    architecture "x86_64"
    cppdialect "C++latest"
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
    cppdialect "C++latest"
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

project "utils"
    language "C++"
    architecture "x86_64"
    cppdialect "C++latest"
    kind "StaticLib"

    location "%{prj.name}"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"
    }
