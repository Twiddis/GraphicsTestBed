#pragma once
#include "Singletons/Singleton.hpp"
#include "Assimp/Importer.hpp"

#include "Resources/Model/Model.hpp"
#include "Resources/Shader/ShaderPipe.hpp"

namespace CayleeEngine::sys
{
class ResourceLoader : public Singleton<ResourceLoader>
{
public:
  ResourceLoader();
  ~ResourceLoader();

  void LoadModel(const std::string &filepath);
  res::ShaderPipe::Key LoadShaderProgram(const std::string &foldername);

private:
  std::unique_ptr<Assimp::Importer> mAssimpImporter;
};
}