#include "precompiled.hpp"
#include "SceneView.hpp"

#include "Singletons/ResourceLoader/ResourceLoader.hpp"
#include "Singletons/InputManager/InputManager.hpp"

#include "Resources/Entity/Entity.hpp"
#include "Resources/Camera/Camera.hpp"
#include "Resources/ShaderResource/RenderTarget.hpp"
#include "Resources/Texture/Texture.hpp"
#include "Resources/Light/Light.hpp"

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
static res::ShaderPipe::Key gShaderProgramLocalLight;

static res::Camera::Key gCamera;

static res::D3DBuffer::Key gEntityTransformBuffer;
static res::D3DBuffer::Key gGBufferRenderFlagsBuffer;
static res::D3DBuffer::Key gLightBuffer;
static res::D3DBuffer::Key gViewBuffer;

static ID3D11SamplerState *gSamplerState = nullptr;

static res::Model::Key gModelBunny;
static res::Model::Key gModelCube;
static res::Model::Key gModelSphere;

static res::Texture::Key gTextureWood;
static res::Texture::Key gTextureFur;

static res::Entity::Key gTestEntity;
static res::Entity::Key gTestEntity2;
static res::Entity::Key gTestEntity3;
static res::Entity::Key gFloorEntity;

static std::vector<res::Entity::Key> gLightEntities;

static res::RenderTarget::Key gBuffer;

static res::Light::Key gMainLight;
static std::vector<res::Light::Key> gLights;
//static std::vector<res::Entity::Key> gLightEntities;

static int gGBufferRenderSetting = 0;
SceneView::SceneView()
{
  ResourceLoader::Initialize();
  
  gModelBunny = ResourceLoader::GetInstance()->LoadModel("stanford-bunny.fbx");
  gModelCube = ResourceLoader::GetInstance()->LoadModel("cubeycube.fbx");
  gModelSphere = ResourceLoader::GetInstance()->LoadModel("sphereysphere.fbx");

  gTextureWood = res::Texture::Create(L"../res/TexturesCom_WoodFine0086_7_seamless_S.dds");
  gTextureFur = res::Texture::Create(L"../res/TexturesCom_Fur0007_M.dds");

  gShaderProgramDefault = ResourceLoader::GetInstance()->LoadShaderProgram("DefaultShader");
  gShaderProgramGBuffer = ResourceLoader::GetInstance()->LoadShaderProgram("GBufferShader");
  gShaderProgramPhong = ResourceLoader::GetInstance()->LoadShaderProgram("PhongShader");
  gShaderProgramLocalLight = ResourceLoader::GetInstance()->LoadShaderProgram("LocalLightShader");

  gEntityTransformBuffer = res::D3DBuffer::FindBufferWIthName("EntityTransform.hlsl");
  gGBufferRenderFlagsBuffer = res::D3DBuffer::FindBufferWIthName("GBufferRenderFlags.hlsl");
  gLightBuffer = res::D3DBuffer::FindBufferWIthName("Light.hlsl");
  gViewBuffer = res::D3DBuffer::FindBufferWIthName("ViewVector.hlsl");

  gCamera = res::Camera::Create();

  gMainLight = res::Light::Create();

  //gTestEntity = res::Entity::Create();
  //{
  //  gTestEntity->mModel = gModelBunny;
  //  gTestEntity->mShaderProgram = gShaderProgramGBuffer;
  //  gTestEntity->mTexture = gTextureWood;
  //  gTestEntity->mPosition = Vec3(0.0f, 0.5f, -4.0f);
  //  gTestEntity->mScale = Vec3(1.0f, 1.0f, 1.0);
  //  gTestEntity->mRotation = Vec3(0.0f, 0.0f, 0.0f);
  //}

  //gTestEntity2 = res::Entity::Create();
  //{
  //  gTestEntity2->mModel = gModelBunny;
  //  gTestEntity2->mShaderProgram = gShaderProgramGBuffer;
  //  gTestEntity2->mTexture = gTextureWood;
  //  gTestEntity2->mPosition = Vec3(1.0f, 2.5f, 0.0f);
  //  gTestEntity2->mScale = Vec3(1.0f, 1.0f, 1.0);
  //  gTestEntity2->mRotation = Vec3(1.0f, 0.0f, 0.0f);
  //}

  //gTestEntity3 = res::Entity::Create();
  //{
  //  gTestEntity3->mModel = gModelBunny;
  //  gTestEntity3->mShaderProgram = gShaderProgramGBuffer;
  //  gTestEntity3->mTexture = gTextureWood;
  //  gTestEntity3->mPosition = Vec3(3.0f, 0.5f, 3.0f);
  //  gTestEntity3->mScale = Vec3(1.0f, 1.0f, 1.0);
  //  gTestEntity3->mRotation = Vec3(0.0f, 2.0f, 0.0f);
  //}

  gFloorEntity = res::Entity::Create();
  {
    gFloorEntity->mModel = gModelCube;
    gFloorEntity->mShaderProgram = gShaderProgramGBuffer;
    gFloorEntity->mTexture = gTextureWood;
    gFloorEntity->mPosition = Vec3(0.0f, -0.5f, 0.0f);
    gFloorEntity->mScale = Vec3(30.0f, 0.5f, 30.0);
    gFloorEntity->mRotation = Vec3(0.0f, 0.0f, 0.0f);
  }

  gMainLight = res::Light::Create();
  gMainLight->mPosition = Vec3(0.0f, 20.0f, 0.0f);

  const size_t light_count = 100;
  const Vec2 x_range(-15.0f, 15.0f);
  const Vec2 y_range(1.0f, 3.0f);
  const Vec2 z_range(-15.0f, 15.0f);
  const Vec2 r_range(5.0f, 8.0f);

  std::srand(static_cast<size_t>(std::time(nullptr)));
  
  for (size_t i = 0; i < light_count; ++i)
  {
    gLights.push_back(res::Light::Create());

    float xs = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
    float ys = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
    float zs = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
    float rs = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);

    float x = (1.0f - xs) * x_range.x + xs * x_range.y;
    float y = (1.0f - ys) * y_range.x + ys * y_range.y;
    float z = (1.0f - zs) * z_range.x + zs * z_range.y;
    float r = (1.0f - rs) * z_range.x + rs * z_range.y;

    gLights.back()->mPosition = Vec3(x, y, z);
    gLights.back()->mColor = Vec4(xs, zs, 1.0f, 1.0f);
    gLights.back()->mRadius = r;
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
  gLightBuffer->MapData(&(gMainLight->mColor));

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

  gShaderProgramLocalLight->Bind();
  gViewBuffer->MapData(&gCamera->mPosition);

    // Add on local lighting
  for (auto &light : gLights)
  {
    gLightBuffer->MapData(&(light->mColor));
    devcon->Draw(4, 0);
  }

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
 