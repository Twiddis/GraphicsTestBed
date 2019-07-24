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
  const std::string CONSTANT_BUFFER_PATH = SHADER_PATH + "ConstantBuffers/";

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
  LoadAllConstantBuffers();
}

ResourceLoader::~ResourceLoader()
{
}


res::Model::Key ResourceLoader::LoadModel(const std::string &filepath)
{
  
  const aiScene *scene = mAssimpImporter->ReadFile(RESOURCE_PATH + filepath, 
                                                    aiProcessPreset_TargetRealtime_Fast);
  
  err::AssertWarn(scene, "Warning: Could not load model from file %s\nError Message: %s", 
                  filepath.c_str(), mAssimpImporter->GetErrorString());
  
  if (!scene)
    return res::Model::Key();
    
  res::Model::Key new_model = res::Model::Create();


  // Normalize mesh data
  float min_x = FLT_MAX; float max_x = 0.0f;
  float min_y = FLT_MAX; float max_y = 0.0f;
  float min_z = FLT_MAX; float max_z = 0.0f;

  for (size_t i = 0; i < scene->mNumMeshes; ++i)
  {
    aiMesh *loaded_mesh = scene->mMeshes[i];

    for (size_t j = 0; j < loaded_mesh->mNumVertices; ++j) 
    {
      max_x = max(max_x, loaded_mesh->mVertices[j].x);
      max_y = max(max_y, loaded_mesh->mVertices[j].y);
      max_z = max(max_z, loaded_mesh->mVertices[j].z);

      min_x = min(min_x, loaded_mesh->mVertices[j].x);
      min_y = min(min_y, loaded_mesh->mVertices[j].y);
      min_z = min(min_z, loaded_mesh->mVertices[j].z);
    }

    loaded_mesh->mNormals[i].Normalize();
  }

  float length_x = max_x - min_x;
  float length_y = max_y - min_y;
  float length_z = max_z - min_z;

    // NOTE: Make better when have time
  for (size_t i = 0; i < scene->mNumMeshes; ++i) {
    aiMesh *loaded_mesh = scene->mMeshes[i];
    res::Mesh::Key new_mesh = res::Mesh::Create();

      // Scale to a bounding box of -0.5f, 0.5f
    for (size_t j = 0; j < loaded_mesh->mNumVertices; ++j) {
      loaded_mesh->mVertices[j].x /= length_x;
      loaded_mesh->mVertices[j].y /= length_y;
      loaded_mesh->mVertices[j].z /= length_z;
    }
     
      // Does nothing if data is null
    new_mesh->CreateVertexBuffer(res::Mesh::Position, loaded_mesh->mVertices, loaded_mesh->mNumVertices);
    new_mesh->CreateVertexBuffer(res::Mesh::Normal, loaded_mesh->mNormals, loaded_mesh->mNumVertices);
    new_mesh->CreateVertexBuffer(res::Mesh::UV, loaded_mesh->mTextureCoords[0], loaded_mesh->mNumVertices);
    
    new_mesh->CreateIndexBuffer(loaded_mesh->mFaces, loaded_mesh->mNumFaces);
    new_mesh->Build();

    new_model->AppendMesh(new_mesh);
  }

  mAssimpImporter->FreeScene();

  return new_model;
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

    //
    vertex_shader->SearchAndAssignBuffers(vertex_path);
    //

    if (!vertex_shader.IsValid()) {
      err::AssertWarn(false, "Warning! Unable to create vertex shader: %S", vertex_path.c_str());
      vertex_shader.Destroy();
    }

    shader_pipe->AttachShader(res::Shader::Vertex, vertex_shader);
  }

  if (b_pixel) {
    res::PixelShader::Key pixel_shader = res::PixelShader::Create();
    pixel_shader->LoadShader(pixel_path);
    
    //
    pixel_shader->SearchAndAssignBuffers(pixel_path);
    //
    if (!pixel_shader.IsValid()) {
      err::AssertWarn(false, "Warning! Unable to create vertex shader: %S", pixel_path.c_str());
      pixel_shader.Destroy();
    }

    shader_pipe->AttachShader(res::Shader::Pixel, pixel_shader);
  }

  return shader_pipe;
}

res::D3DBuffer::Key ResourceLoader::LoadConstantBuffer(const std::string &filepath)
{
  std::ifstream in_file(filepath);
  std::stringstream str_stream;

  str_stream << in_file.rdbuf();
  std::string file_buffer = str_stream.str();

  const std::string search = "// byte_width: ";
  size_t search_pos = file_buffer.find(search) + search.length();
  size_t search_end = file_buffer.find('\n', search_pos);

  std::string found = file_buffer.substr(search_pos, search_end - search_pos);

  size_t filename_pos = filepath.find_last_of("/") + 1;
  std::string filename = filepath.substr(filename_pos);

  return res::D3DBuffer::Create(filename, std::stoi(found));
}

void ResourceLoader::LoadAllConstantBuffers()
{
  for (auto &file : fs::recursive_directory_iterator(CONSTANT_BUFFER_PATH))
  {
    if (file.path().extension() == ".hlsl") {
      res::D3DBuffer::Key buffer = LoadConstantBuffer(file.path().string());
      mConstantBuffers.emplace(file.path().filename().string(), buffer);
    }
  }
}

}
