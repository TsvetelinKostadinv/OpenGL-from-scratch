workspace "OpenGL-from-scratch"
  configurations { "Debug", "Release" }

  platforms
  {
    "Win32",
    "Win64"
  }
  startproject "OpenGL-from-scratch"
  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

  filter { "platforms:Win32" }
    system "Windows"
    architecture "x86"

  filter { "platforms:Win64" }
    system "Windows"
    architecture "x86_64"

  project "OpenGL-from-scratch"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
      "src/**.h",
      "src/**.cpp",
      ".clang-format"
    }

    defines
    {
      "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
      "src"
    }

    filter "configurations:Debug"
      defines "DISANA_DEBUG"
      runtime "Debug"
      symbols "on"

    filter "configurations:Release"
      defines "DISANA_RELEASE"
      runtime "Release"
      optimize "on"
