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
        "xor_decryption/src",
        "utils/src"
    }

    links {
        "xor_decryption",
        "utils"
    }

    -- -- todo: good?
    -- sysincludedirs {
    --     "xor_decryption/src"
    -- }

project "xor_encryption"
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

project "xor_decryption"
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

    includedirs {
        "utils/src"
    }

    links {
        "utils"
    }

    -- -- todo: good
    -- includedirs {
    --     "src/"
    -- }

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