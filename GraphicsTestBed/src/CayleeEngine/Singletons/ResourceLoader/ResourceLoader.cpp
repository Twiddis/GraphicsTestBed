#include "precompiled.hpp"
#include "ResourceLoader.hpp"

#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::sys
{
  ResourceLoader::ResourceLoader()
  {
    mAssimpImporter = std::make_unique<Assimp::Importer>();
  }

  ResourceLoader::~ResourceLoader()
  {
    
  }

  void ResourceLoader::Update(float)
  {
  }

  void ResourceLoader::LoadAllResources()
  {
    LoadModels();
  }

  void ResourceLoader::LoadModels()
  {
  }
  void ResourceLoader::LoadModel()
  {
    
  }
}
