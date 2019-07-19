#include "precompiled.hpp"
#include "ResourceLoader.hpp"

#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include "Resources/Model/Model.hpp"
#include "Singletons/D3D/D3D.hpp"

namespace
{
  const std::string RESOURCE_PATH = "../res/";
}
namespace CayleeEngine::sys
{
  ResourceLoader::ResourceLoader() : mAssimpImporter(nullptr)
  {
    mAssimpImporter = std::make_unique<Assimp::Importer>();
  }

  ResourceLoader::~ResourceLoader()
  {
  }

  void ResourceLoader::LoadAllResources()
  {
    LoadModels();
  }

  void ResourceLoader::LoadModels()
  {
    LoadModel(RESOURCE_PATH + "standford_bunny.fbx");
  }

  void ResourceLoader::LoadModel(const std::string &filepath)
  {
    const aiScene *scene = mAssimpImporter->ReadFile(filepath, aiProcessPreset_TargetRealtime_Fast);
    err::AssertWarn(scene, "Warning: Could not load model from file %s", filepath.c_str());

    if (!scene)
      return;
    
    res::Model::Key new_model = res::Model::Create();
    
      // NOTE: Make better when have time
    for (size_t i = 0; i < scene->mNumMeshes; ++i) {
      res::Mesh::Key new_mesh = res::Mesh::Create();
      aiMesh *loaded_mesh = scene->mMeshes[i];

      aiVector3D *vertices = loaded_mesh->mVertices;
      aiVector3D *normals  = loaded_mesh->mNormals;
      aiVector3D *UVs = loaded_mesh->mTextureCoords[0];

      std::memcpy(new_mesh->mPositions.data(), vertices, loaded_mesh->mNumVertices);
      std::memcpy(new_mesh->mNormals.data(), normals, loaded_mesh->mNumVertices);
  
      if (loaded_mesh->HasTextureCoords(0)) {
        for (size_t j = 0; j < loaded_mesh->mNumUVComponents[0]; ++j)
          new_mesh->mUVs.push_back(Vec2(UVs[j].x, UVs[j].y));
      }
    }

    mAssimpImporter->FreeScene();
  }
}
