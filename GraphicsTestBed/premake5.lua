function RemoveDirs(pattern)
  local matches = os.matchdirs(pattern)
  local count = 0;
  for key,value in pairs(matches) do
    os.rmdir(value)
    count = count + 1 -- sorry, no increment operator in LUA :(
  end
  return count
end

-- Find windows sdk version
-- https://github.com/premake/premake-core/issues/935
function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

-- Adding new Clean Action
newaction {
  trigger     = "clean",
  description = "clean the build",
  execute     = function ()
    print("cleaning the build...")
    local removed = RemoveDirs(".\\project_*") + RemoveDirs(".\\bin*")
    os.rmdir(".\\.vs")
    printf("done. %d files removed", removed)
  end
}


workspace "GraphicsTestBed"
  configurations { "Debug", "Release" } -- Optimization/General config mode in VS
  platforms { "x86", "x64" }

  local project_action = "UNDEFINED"
  if _ACTION ~= nill then
    project_action = _ACTION
  end

  location( "project_" .. project_action )

  -------------------------------
  -- [ COMPILER/LINKER CONFIG] --
  -------------------------------
  flags "FatalWarnings" -- comment if you don't want warnings to count as errors
  warnings "Extra"

  characterset ("MBCS")
  -- Here we are setting up what differentiates the configurations that we called "Debug" and "Release"
  filter "configurations:Debug"
    defines { "DEBUG", "_CRT_SECURE_NO_WARNINGS" }
    symbols  "On"
  filter{}

  filter "configurations:Release"
    defines { "NDEBUG", "_CRT_SECURE_NO_WARNINGS" }
    optimize "On"
  filter{}

  filter { "platforms:x86" }
    architecture "x86"
  filter {}
  filter { "platforms:x64" }
    architecture "x64"
  filter {}

  -- You can AND filters as follows:
  filter { "system:windows", "action:vs*"}
    flags         { "MultiProcessorCompile", "NoMinimalRebuild" }
    linkoptions   { "/ignore:4099", "/NODEFAULTLIB:library" }
    systemversion(os.winSdkVersion() .. ".0")
  filter {}  -- clear filter when you know you no longer need it!

  -------------------------------
  -- [ Graphics Engine ] --
  -------------------------------
  project "CayleeEngine"
    kind "WindowedApp" -- "WindowedApp" removes console
    language "C++"
    targetdir "bin_%{cfg.platform}_%{cfg.buildcfg}" -- Where the output binary goes. This will be generated when we build from the makefile/visual studio project/etc.
    targetname "CayleeEngine"							-- the name of the executable saved to 'targetdir'
    cppdialect "C++17"

    local SourceDir = "./src/CayleeEngine/";
    files {
      SourceDir .. "**.h",
      SourceDir .. "**.hpp",
      SourceDir .. "**.c",
      SourceDir .. "**.cpp",
      SourceDir .. "**.tpp",
      SourceDir .. "**.inl",
    }

    -- Exclude template files from project (so they don't accidentally get compiled)
    filter { "files:**.tpp" }
      flags {"ExcludeFromBuild"}
    filter {} -- clear filter!

    filter { "files:**.inl" }
      flags {"ExcludeFromBuild"}
    filter {}

    pchheader "precompiled.hpp"
    pchsource(SourceDir .. "precompiled.cpp")

    includedirs { SourceDir, "./dep/inc" }
    libdirs { "./dep/lib" }

    links {
      "d3d11",
      "d3dcompiler",
      "/LZ4/%{cfg.platform}_%{cfg.buildcfg}/liblz4_static",
      "/DirectXTK/%{cfg.platform}_%{cfg.buildcfg}/DirectXTK",
      "/FBX/%{cfg.platform}_%{cfg.buildcfg}/libfbxsdk",
      "/Assimp/%{cfg.platform}_%{cfg.buildcfg}/assimp-vc141-mt"
    }

    prebuildcommands {}
    postbuildcommands {
      "copy ..\\dep\\lib\\FBX\\%{cfg.platform}_%{cfg.buildcfg}\\libfbxsdk.dll ..\\bin_%{cfg.platform}_%{cfg.buildcfg}\\",
      "copy ..\\dep\\lib\\Assimp\\%{cfg.platform}_%{cfg.buildcfg}\\assimp-vc141-mt.dll ..\\bin_%{cfg.platform}_%{cfg.buildcfg}\\"
    }

    filter "platforms:x86"
      links {}

      filter "configurations:Debug"
      filter {}

      filter "configurations:Release"
      filter {}

      postbuildcommands {}

    filter {}

    filter "platforms:x64"
      links {}

      filter "configurations:Debug"
      filter {}

      filter "configurations:Release"
      filter {}

      postbuildcommands {}
    filter {}

  -------------------------------
  -- [ FBX Loader ] --
  -------------------------------
  project "FBXLoader"
    kind "ConsoleApp" -- "WindowedApp" removes console
    language "C++"
    targetdir "bin_%{cfg.buildcfg}_%{cfg.platform}" -- Where the output binary goes. This will be generated when we build from the makefile/visual studio project/etc.
    targetname "FBXLoader"							-- the name of the executable saved to 'targetdir'
    cppdialect "C++17"
    debugdir "."
    flags { "WinMain" }

    local SourceDir = "./src/FBXLoader/";
    files
    {
      SourceDir .. "**.h",
      SourceDir .. "**.hpp",
      SourceDir .. "**.c",
      SourceDir .. "**.cpp",
      SourceDir .. "**.tpp",
      SourceDir .. "**.inl",
    }

    -- Exclude template files from project (so they don't accidentally get compiled)
    filter { "files:**.tpp" }
      flags {"ExcludeFromBuild"}
    filter {} -- clear filter!


    filter { "files:**.inl" }
      flags {"ExcludeFromBuild"}
    filter {}

    includedirs { SourceDir }
    libdirs { "./dep/lib" }
    links {
      "/LZ4/%{cfg.platform}_%{cfg.buildcfg}/liblz4_static",
      "/DirectXTK/%{cfg.platform}_%{cfg.buildcfg}/DirectXTK",
      "/FBX/%{cfg.platform}_%{cfg.buildcfg}/libfbxsdk"
    }

    prebuildcommands {}
    postbuildcommands {
      "copy ..\\dep\\lib\\FBX\\%{cfg.platform}_%{cfg.buildcfg}\\libfbxsdk.dll ..\\bin_%{cfg.platform}_%{cfg.buildcfg}\\"
    }

    filter "platforms:x86"
      links {}

      filter "configurations:Debug"
      filter {}

      filter "configurations:Release"
      filter {}

      postbuildcommands {}

    filter {}

    filter "platforms:x64"
      links {}

      filter "configurations:Debug"
      filter {}

      filter "configurations:Release"
      filter {}

      postbuildcommands {}
    filter {}
