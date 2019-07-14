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

  void Update(float dt);

  void LoadModel();

private:
  void LoadAllResources();

  void LoadModels();

private:

  std::unique_ptr<Assimp::Importer> mAssimpImporter;
};
}