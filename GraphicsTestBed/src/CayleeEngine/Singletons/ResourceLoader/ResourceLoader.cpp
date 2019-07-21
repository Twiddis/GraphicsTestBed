#include "precompiled.hpp"
#include "ResourceLoader.hpp"

#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

#include "Singletons/D3D/D3D.hpp"
#include "Resources/Shader/VertexShader.hpp"
#include "Resources/Shader/PixelShader.hpp"

#include <filesystem>
namespace fs = std::filesystem;

namespace
{
  const std::string RESOURCE_PATH = "../res/";
  const std::string SHADER_PATH = "../src/CayleeEngine/Shaders/";

  namespace FileExtensions
  {
    const std::string VERTEX_SHADER = ".vert";
    const std::string PIXEL_SHADER = ".pixel";
    const std::string COMPUTE_SHADER = ".comp";
  }
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


void ResourceLoader::LoadModel(const std::string &filepath)
{
  const aiScene *scene = mAssimpImporter->ReadFile(RESOURCE_PATH + filepath, 
                                                    aiProcessPreset_TargetRealtime_Fast);
    
  err::AssertWarn(scene, "Warning: Could not load model from file %s\nError Message: %s", 
                  filepath.c_str(), mAssimpImporter->GetErrorString());

  if (!scene)
    return;
    
  res::Model::Key new_model = res::Model::Create();
    
    // NOTE: Make better when have time
  for (size_t i = 0; i < scene->mNumMeshes; ++i) {
    res::Mesh::Key new_mesh = res::Mesh::Create();
    aiMesh *loaded_mesh = scene->mMeshes[i];
      
    loaded_mesh->mVertices->NormalizeSafe();

      // Does nothing if data is null
    new_mesh->CreateVertexBuffer(res::Mesh::Position, loaded_mesh->mVertices, loaded_mesh->mNumVertices);
    new_mesh->CreateVertexBuffer(res::Mesh::Normal, loaded_mesh->mNormals, loaded_mesh->mNumVertices);
    new_mesh->CreateVertexBuffer(res::Mesh::UV, loaded_mesh->mTextureCoords[0], loaded_mesh->mNumVertices);
  }

  mAssimpImporter->FreeScene();
}

res::ShaderPipe::Key ResourceLoader::LoadShaderProgram(const std::string &foldername)
{
  fs::directory_iterator folder(SHADER_PATH + foldername);

    // Find all files in folder
  bool b_vertex = false;
  bool b_pixel = false;
  bool b_compute = false;

  std::wstring vertex_path;
  std::wstring pixel_path;
  std::wstring compute_path;

  for (auto &file : folder) 
  {
    std::string ext = file.path().extension().string();

    if (ext == FileExtensions::VERTEX_SHADER) {
      vertex_path = file.path().wstring();
      b_vertex = true;
    }

    else if (ext == FileExtensions::PIXEL_SHADER) {
      pixel_path = file.path().wstring();
      b_pixel = true;
    }

    else if (ext == FileExtensions::COMPUTE_SHADER) {
      compute_path = file.path().wstring();
      b_compute = true;
    }
  }

  bool is_valid_shader_program = (b_vertex && b_pixel) || b_compute;

  if (!is_valid_shader_program) {
    err::AssertWarn(false, "Warning! Invalid shaders in shader program: %s", foldername.c_str());
    return res::ShaderPipe::Key();
  }

  res::ShaderPipe::Key shader_pipe = res::ShaderPipe::Create();

  if (b_vertex) {
    res::VertexShader::Key vertex_shader = res::VertexShader::Create();
    vertex_shader->LoadShader(vertex_path);

    if (!vertex_shader.IsValid()) {
      err::AssertWarn(false, "Warning! Unable to create vertex shader: %S", vertex_path.c_str());
      vertex_shader.Destroy();
    }

    shader_pipe->AttachShader(res::Shader::Vertex, vertex_shader);
  }

  if (b_pixel) {
    res::PixelShader::Key pixel_shader = res::PixelShader::Create();
    pixel_shader->LoadShader(pixel_path);

    if (!pixel_shader.IsValid()) {
      err::AssertWarn(false, "Warning! Unable to create vertex shader: %S", pixel_path.c_str());
      pixel_shader.Destroy();
    }

    shader_pipe->AttachShader(res::Shader::Pixel, pixel_shader);
  }

  return shader_pipe;
}

}
