#include "precompiled.hpp"
#include "SceneView.hpp"

#include "Singletons/ResourceLoader/ResourceLoader.hpp"

namespace CayleeEngine::sys
{

SceneView::SceneView()
{
  ResourceLoader::Initialize();
  ResourceLoader::GetInstance()->LoadModel("stanford-bunny.fbx");
  ResourceLoader::GetInstance()->LoadShaderProgram("DefaultShader");

}

SceneView::~SceneView()
{
  ResourceLoader::Shutdown();
}

void SceneView::StartFrame()
{
}

void SceneView::Update(float)
{
}
void SceneView::EndFrame()
{
}
}
 