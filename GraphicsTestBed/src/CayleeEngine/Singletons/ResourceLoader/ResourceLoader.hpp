#pragma once
#include "Singletons/Singleton.hpp"
#include "Assimp/Importer.hpp"

#include "Resources/Model/Model.hpp"
#include "Resources/Shader/ShaderPipe.hpp"
#include "Resources/D3DBuffer/D3DBuffer.hpp"

namespace CayleeEngine::sys
{
class ResourceLoader : public Singleton<ResourceLoader>
{
public:
  ResourceLoader();
  ~ResourceLoader();

  res::Model::Key LoadModel(const std::string &filepath);
  res::ShaderPipe::Key LoadShaderProgram(const std::string &foldername);
  res::D3DBuffer::Key LoadConstantBuffer(const std::string &filepath);

private:
  std::unique_ptr<Assimp::Importer> mAssimpImporter;

  void LoadAllConstantBuffers();
  std::unordered_map<std::string, res::D3DBuffer::Key> mConstantBuffers;
};
}