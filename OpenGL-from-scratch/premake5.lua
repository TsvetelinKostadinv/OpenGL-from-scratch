project "OpenGL-from-scratch"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  staticruntime "off"
  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

  warnings "Extra"
  rtti "Off"
  exceptionhandling "Off"

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

  flags {
    "FatalCompileWarnings",
    "MultiProcessorCompile",
    "No64BitChecks",
    "NoImplicitLink",
    "NoImportLib",
    "ShadowedVariables"
  }

  includedirs
  {
    "src",
    "%{wks.location}/ext/glfw/include",
    "%{wks.location}/ext/glad/include"
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
    flags
    {
      "LinkTimeOptimization",
      "NoBufferSecurityCheck"
    }
