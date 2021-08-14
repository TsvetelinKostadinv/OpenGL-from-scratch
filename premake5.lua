workspace "OpenGL-from-scratch"
  configurations { "Debug", "Release" }

  platforms
  {
    "Win64"
  }
  startproject "OpenGL-from-scratch"
  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


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
      "src",
      "ext/glfw/include",
      "ext/glad/include"
    }

    links
    {
      "GLFW",
      "GLAD"
    }

    filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

    filter "configurations:Release"
      runtime "Release"
      optimize "on"

include "ext/glfw.lua"
include "ext/glad.lua"
