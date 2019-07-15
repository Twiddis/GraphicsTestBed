#pragma once
#include "Singletons/Singleton.hpp"
#include "Assimp/Importer.hpp"

namespace CayleeEngine::sys
{
class ResourceLoader : public Singleton<ResourceLoader>
{
public:
  ResourceLoader();
  ~ResourceLoader();

  void LoadModel(const std::string &filepath);

private:
  void LoadAllResources();

  void LoadModels();

private:

  std::unique_ptr<Assimp::Importer> mAssimpImporter;
};
}