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

include "OpenGL-from-scratch/premake5.lua"
include "ext/glfw.lua"
include "ext/glad.lua"
