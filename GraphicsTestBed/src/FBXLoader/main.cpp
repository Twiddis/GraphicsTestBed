/*!*****************************************************************************
\file   main.cpp
\author Caylee Ayson
\par    Digipen Email: c.ayson\@digipen.edu
\par    DigiPen login: c.ayson
\date   09/28/2018
\brief
  Main entrypoint to the Asset Loader.

\par Copyright (C) DigiPen Institute of Technology 2017

*******************************************************************************/
#include <experimental/filesystem>
#include <fbxsdk.h>
#include <fstream>
#include <iostream>
#include <Windows.h>

#include "AssetManager/FBXFileLoader.h"
#include "../RoseEngine/Trace/Trace.h"

namespace fs = std::experimental::filesystem;
using namespace ntr;

static bool gTraceEnabled = false;

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int argc)
{
  AllocConsole();
  freopen("CONOUT$", "w", stdout);

  if (argc > 1) {
    gTraceEnabled = true;
  }
  
  
  Trace::CreateTrace(Trace::All);
  Trace::AddStream(&std::cout);
  Trace::AddFile("fbx_trace.log");

  AssetLoader::FBXFileLoader loader;
  traceD << "Packaging Assets..." << std::endl << std::endl;

  char buffer[MAX_PATH];
  GetModuleFileName(NULL, buffer, MAX_PATH);
  std::string::size_type pos = std::string(buffer).find_last_of("\\/");
  std::string assets_path = std::string(buffer).substr(0, pos) + "\\assets";
  std::string output_dir = std::string(buffer).substr(0, pos) + "\\packaged_assets";
  fs::create_directory(output_dir);

  for (auto &file : fs::recursive_directory_iterator(assets_path))
  {
    if (file.path().extension().string() != ".fbx")
      continue;

    std::string input_file = file.path().filename().string();
    std::string output_file = input_file.substr(0, input_file.find_last_of('.')) + ".mdl";

    std::string input_path = file.path().string();
    std::string output_path = output_dir + "\\" + output_file;

    FbxScene *scene = NULL;

    if (!loader.LoadFile(input_path, scene)) {
      traceW << "ERROR: Unable to package file: " << input_file << std::endl;
      continue;
    }

    //loader.PrintTree(scene);

    loader.Preprocess(scene, output_dir + "\\" + output_file);

    traceD << "File successfully packaged: " << input_file << std::endl << std::endl;
    scene->Destroy();
  }

  system("pause");

  return 0;
}