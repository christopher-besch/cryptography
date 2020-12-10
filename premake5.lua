outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "cryptography"
   startproject "xor_encryption"

   configurations { "Debug", "Release" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
   filter "configurations:Release"
      defines { "RELEASE" }
      optimize "On"


project "xor_encryption"
   language "C++"
   architecture "x86_64"
   cppdialect "C++17"
   kind "ConsoleApp"

   location "xor_encryption"
   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   files {
      "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"
   }


project "xor_decryption"
   language "C++"
   architecture "x86_64"
   cppdialect "C++17"
   kind "ConsoleApp"

   location "xor_decryption"
   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   files {
      "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"
   }
