#include "precompiled.hpp"
#include "SceneView.hpp"

#include "Singletons/ResourceLoader/ResourceLoader.hpp"
#include "Singletons/InputManager/InputManager.hpp"

#include "Resources/Entity/Entity.hpp"
#include "Resources/Camera/Camera.hpp"
#include "Resources/ShaderResource/RenderTarget.hpp"
#include "Resources/Texture/Texture.hpp"

namespace CayleeEngine::sys
{

struct BEntityTransform
{
  Mat model;
  Mat view;
  Mat projection;
  Mat trans_inv_model;
};


static res::ShaderPipe::Key gShaderProgramDefault;
static res::ShaderPipe::Key gShaderProgramGBuffer;
static res::ShaderPipe::Key gShaderProgramPhong;

static res::Camera::Key gCamera;
static res::D3DBuffer::Key gEntityTransformBuffer;
static res::D3DBuffer::Key gGBufferRenderFlagsBuffer;

static ID3D11SamplerState *gSamplerState = nullptr;


static res::Model::Key gModelBunny;
static res::Model::Key gModelCube;

static res::Texture::Key gTextureWood;

static res::Entity::Key gTestEntity;
static res::Entity::Key gFloorEntity;

static res::RenderTarget::Key gBuffer;

static int gGBufferRenderSetting = 0;
SceneView::SceneView()
{
  ResourceLoader::Initialize();
  
  gModelBunny = ResourceLoader::GetInstance()->LoadModel("stanford-bunny.fbx");
  gModelCube = ResourceLoader::GetInstance()->LoadModel("cubeycube.fbx");

  gTextureWood = res::Texture::Create(L"../res/TexturesCom_WoodFine0086_7_seamless_S.dds");

  gShaderProgramDefault = ResourceLoader::GetInstance()->LoadShaderProgram("DefaultShader");
  gShaderProgramGBuffer = ResourceLoader::GetInstance()->LoadShaderProgram("GBufferShader");
  gShaderProgramPhong = ResourceLoader::GetInstance()->LoadShaderProgram("PhongShader");

  gEntityTransformBuffer = res::D3DBuffer::FindBufferWIthName("EntityTransform.hlsl");
  gGBufferRenderFlagsBuffer = res::D3DBuffer::FindBufferWIthName("GBufferRenderFlags.hlsl");
  
  gCamera = res::Camera::Create();

  gTestEntity = res::Entity::Create();
  {
    gTestEntity->mModel = gModelBunny;
    gTestEntity->mShaderProgram = gShaderProgramGBuffer;
    gTestEntity->mPosition = Vec3(0.0f, 0.5f, -4.0f);
    gTestEntity->mScale = Vec3(1.0f, 1.0f, 1.0);
    gTestEntity->mRotation = Vec3(0.0f, 0.0f, 0.0f);
  }

  gFloorEntity = res::Entity::Create();
  {
    gFloorEntity->mModel = gModelCube;
    gFloorEntity->mShaderProgram = gShaderProgramGBuffer;
    gFloorEntity->mTexture = gTextureWood;
    gFloorEntity->mPosition = Vec3(0.0f, -0.5f, 0.0f);
    gFloorEntity->mScale = Vec3(10.0f, 0.5f, 10.0);
    gFloorEntity->mRotation = Vec3(0.0f, 0.0f, 0.0f);
  }

    // Position
    // Normal
    // Diffuse
    // Specular
  gBuffer = res::RenderTarget::Create(4, 1920, 1080);

  D3D11_SAMPLER_DESC sampler_desc;
  {
    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.MipLODBias = 0;
    sampler_desc.MaxAnisotropy = 1;
    sampler_desc.BorderColor[0] = 0.0f;
    sampler_desc.BorderColor[1] = 0.0f;
    sampler_desc.BorderColor[2] = 0.0f;
    sampler_desc.BorderColor[3] = 1.0f;
    sampler_desc.MinLOD = 0;
    sampler_desc.MaxLOD = 0;
  };

  err::HRFail(D3D::GetInstance()->mDevice->CreateSamplerState(&sampler_desc, &gSamplerState),
              "SAMPLER STATE CREATION FUCKED UP");

  D3D::GetInstance()->mDeviceContext->PSSetSamplers(0, 1, &gSamplerState);
}

SceneView::~SceneView()
{
  SafeRelease(gSamplerState);
  ResourceLoader::Shutdown();
}

void SceneView::StartFrame()
{
  gBuffer->ClearRenderTarget();

  gCamera->GenerateViewMatrix();
  gCamera->GenerateProjectionMatrix();

  Input();
}

void SceneView::Update(float)
{
  BEntityTransform trans_buffer;

  trans_buffer.view = gCamera->GetViewMatrix().Transpose();
  trans_buffer.projection = gCamera->GetProjectionMatrix().Transpose();

    // Draw Entities
  gBuffer->BindAsRenderTarget();

  for (auto &entity : res::Entity::GetResources()) {
    trans_buffer.model = entity.second->GetTransform().Transpose();

      // You always have to transpose before putting it in a constant buffer,
      // BUUT since this is the inverse transpose...
    trans_buffer.trans_inv_model = entity.second->GetTransform().Invert();


    if (entity.second->mTexture.IsValid())
      entity.second->mTexture->Bind(0, res::Shader::Pixel);
    else
      res::Texture::ClearBinding(0, res::Shader::Pixel);
      
    entity.second->mShaderProgram->Bind();
    gEntityTransformBuffer->MapData(&trans_buffer);

    entity.second->mModel->Draw();
  }


    // Draw GBuffer
  gGBufferRenderFlagsBuffer->MapData(&gGBufferRenderSetting);

  ID3D11DeviceContext *devcon = D3D::GetInstance()->mDeviceContext;
  gShaderProgramPhong->Bind();

  gBuffer->BindAsResourceView(0);

    // Save old topology
  D3D11_PRIMITIVE_TOPOLOGY topology;
  devcon->IAGetPrimitiveTopology(&topology);
  devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

  // Render full screen quad:
  //   This works without binding any buffers because the FSQ_renderer shader
  //   only uses "Vertex ID" to determine uv coords, pos, etc.
  D3D::GetInstance()->BindRenderTarget();
  devcon->Draw(4, 0);

  // Reset old topology
  devcon->IASetPrimitiveTopology(topology);
}
void SceneView::EndFrame()
{
    // Reset the matrix flag for entities
  for (auto &res : res::Entity::GetResources())
    res.second->mIsTransformCalculated = false;

  DirectX::Mouse::Get().ResetScrollWheelValue();
}

void SceneView::Input()
{
  using namespace DirectX;
  auto mouse = Mouse::Get().GetState();
  auto kb = Keyboard::Get().GetState();
  
  mouse_tracker.Update(mouse);
  kb_tracker.Update(kb);

    // Camera Controla
  if (mouse_tracker.middleButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
    Mouse::Get().SetMode(Mouse::MODE_RELATIVE);
 
  else if (mouse_tracker.middleButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
    Mouse::Get().SetMode(Mouse::MODE_ABSOLUTE);
  
  if (mouse_tracker.middleButton == Mouse::ButtonStateTracker::ButtonState::HELD && mouse.positionMode == Mouse::MODE_RELATIVE) {
    gCamera->PanRight(static_cast<float>(mouse.x) * 0.01f);
    gCamera->PanUp(static_cast<float>(mouse.y) * 0.01f);
  }
  else
    gCamera->ZoomIn(static_cast<float>(mouse.scrollWheelValue) * 0.001f);

  if (mouse_tracker.leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
    Mouse::Get().SetMode(Mouse::MODE_RELATIVE);

  else if (mouse_tracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
    Mouse::Get().SetMode(Mouse::MODE_ABSOLUTE);

  if (mouse.leftButton && mouse.positionMode == Mouse::Mode::MODE_RELATIVE)
    gCamera->RotateCamera(static_cast<float>(mouse.x) * 0.01f, static_cast<float>(mouse.y) * 0.01f);


  if (kb.Right == true)
    gTestEntity->mRotation += Vec3(0.0f, 0.1f, 0.0f);
  if (kb.Up == true)
    gTestEntity->mRotation += Vec3(0.0f, 0.0f, 0.1f);

    // Settings
  if (kb_tracker.IsKeyPressed(Keyboard::D1))
    gGBufferRenderSetting = 1;
  else if (kb_tracker.IsKeyPressed(Keyboard::D2))
    gGBufferRenderSetting = 2;
  else if (kb_tracker.IsKeyPressed(Keyboard::D3))
    gGBufferRenderSetting = 3;
  else if (kb_tracker.IsKeyPressed(Keyboard::D4))
    gGBufferRenderSetting = 4;
  else if (kb_tracker.IsKeyPressed(Keyboard::D0))
    gGBufferRenderSetting = 0;
}
}
 