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

struct BLight
{
  Mat light_vp;
  Vec3 light_pos;
  Vec3 camera_pos;
};


static const int KERNAL_DIAMETER = 33;
static double gGaussianWeights[KERNAL_DIAMETER] = { 0 };
struct BBlurFilter
{
  Vec4 weights[KERNAL_DIAMETER];
  int max_output_size;
  int max_input_size;
};

static res::ShaderPipe::Key gShaderProgramDefault;
static res::ShaderPipe::Key gShaderProgramGBuffer;
static res::ShaderPipe::Key gShaderProgramPhong;
static res::ShaderPipe::Key gShaderProgramLocalLight;
static res::ShaderPipe::Key gShaderProgramShadowLight;
static res::ShaderPipe::Key gShaderProgramHorizontalBlur;
static res::ShaderPipe::Key gShaderProgramVerticalBlur;

static res::Camera::Key gCamera;

static res::D3DBuffer::Key gEntityTransformBuffer;
static res::D3DBuffer::Key gLightBuffer;

static res::D3DBuffer::Key gBlurFilterBuffer;

static ID3D11SamplerState *gSamplerState = nullptr;
static ID3D11RasterizerState *gShadowRasterizerState = nullptr;

static res::Model::Key gModelBunny;
static res::Model::Key gModelCube;
static res::Model::Key gModelSphere;

static res::Texture::Key gTextureWood;
static res::Texture::Key gTextureFur;

static res::Entity::Key gTestEntity;
static res::Entity::Key gTestEntity2;
static res::Entity::Key gTestEntity3;

static res::Entity::Key gFloorEntity;

static res::RenderTarget::Key gBuffer;
static res::RenderTarget::Key gBlurOutputBuffer;
static res::RenderTarget::Key gBlurOutputBuffer2;

//static res::Light::Key gMainLight;
static res::Light::Key gLight1;
static res::Light::Key gLight2;
static std::vector<res::Light::Key> gLights;

static int gGBufferRenderSetting = 0;
static int gRenderSetting = 0;

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
  gShaderProgramShadowLight = ResourceLoader::GetInstance()->LoadShaderProgram("ShadowLightShader");
  gShaderProgramHorizontalBlur = ResourceLoader::GetInstance()->LoadShaderProgram("HorizontalBlurFilter");
  gShaderProgramVerticalBlur = ResourceLoader::GetInstance()->LoadShaderProgram("VerticalBlurFilter");

  gEntityTransformBuffer = res::D3DBuffer::FindBufferWIthName("EntityTransform.hlsl");
  gLightBuffer = res::D3DBuffer::FindBufferWIthName("Light.hlsl");
  gBlurFilterBuffer = res::D3DBuffer::FindBufferWIthName("BlurFilterBuffer.hlsl");

  gCamera = res::Camera::Create();

  gCamera->mPosition = Vec3(0.0f, 10.0f, 15.0f);
  gCamera->RotateCamera(0.0f, -0.8f);
  //gMainLight = res::Light::Create();

  gTestEntity = res::Entity::Create();
  {
    gTestEntity->mModel = gModelBunny;
    gTestEntity->mShaderProgram = gShaderProgramGBuffer;
    gTestEntity->mTexture = gTextureFur;
    gTestEntity->mPosition = Vec3(0.0f, 0.5f, -4.0f);
    gTestEntity->mScale = Vec3(3.0f, 3.0f, 3.0f);
    gTestEntity->mRotation = Vec3(0.0f, 0.0f, 0.0f);
  }

  gTestEntity2 = res::Entity::Create();
  {
    gTestEntity2->mModel = gModelBunny;
    gTestEntity2->mShaderProgram = gShaderProgramGBuffer;
    gTestEntity2->mTexture = gTextureFur;
    gTestEntity2->mPosition = Vec3(0.0f, 0.5f, 4.0f);
    gTestEntity2->mScale = Vec3(4.0f, 4.0f, 4.0f);
    gTestEntity2->mRotation = Vec3(0.0f, 1.0f, 0.0f);
  }

  gTestEntity3 = res::Entity::Create();
  {
    gTestEntity3->mModel = gModelBunny;
    gTestEntity3->mShaderProgram = gShaderProgramGBuffer;
    gTestEntity3->mTexture = gTextureFur;
    gTestEntity3->mPosition = Vec3(0.0f, 0.5f, 12.0f);
    gTestEntity3->mScale = Vec3(5.0f, 5.0f, 5.0f);
    gTestEntity3->mRotation = Vec3(0.0f, 3.0f, 0.0f);
  }

  gFloorEntity = res::Entity::Create();
  {
    gFloorEntity->mModel = gModelCube;
    gFloorEntity->mShaderProgram = gShaderProgramGBuffer;
    gFloorEntity->mTexture = gTextureWood;
    gFloorEntity->mPosition = Vec3(0.0f, -0.5f, 0.0f);
    gFloorEntity->mScale = Vec3(30.0f, 0.5f, 30.0);
    gFloorEntity->mRotation = Vec3(0.0f, 0.0f, 0.0f);
  }

  
  gLight1 = res::Light::Create();
  gLight2 = res::Light::Create();

  gLight1->mPosition = Vec3(14.0f, 10.0f, 15.0f);
  gLight2->mPosition = Vec3(-14.0f, 10.0f, 15.0f);

  gLight1->mLookAtPosition = Vec3(0.0f, 0.0f, -4.0f);
  gLight2->mLookAtPosition = Vec3(0.0f, 0.0f, -4.0f);

  gLights.push_back(gLight1);
  gLights.push_back(gLight2);

    // Position
    // Normal
    // Diffuse
    // Specular
  gBuffer = res::RenderTarget::Create(4, 1920, 1080);
  gBlurOutputBuffer = res::RenderTarget::Create(1, 1024, 1024);
  gBlurOutputBuffer2 = res::RenderTarget::Create(1, 1024, 1024);

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

  {
    D3D11_RASTERIZER_DESC desc;

    desc.AntialiasedLineEnable = false;
    desc.CullMode = D3D11_CULL_BACK;
    desc.DepthBias = 0;
    desc.DepthBiasClamp = 0.0f;
    desc.DepthClipEnable = false;
    desc.FillMode = D3D11_FILL_SOLID;
    desc.FrontCounterClockwise = true; // RH Coordinate System
    desc.MultisampleEnable = false;
    desc.ScissorEnable = false;
    desc.SlopeScaledDepthBias = 0.0f;

    err::HRFail(D3D::GetInstance()->mDevice->CreateRasterizerState(&desc, &gShadowRasterizerState),
      "Unable to Create Rasterizer State");
  }

  // Calculate gaussian weights
  double sum = 0.0f;

  for (int i = 0; i < KERNAL_DIAMETER; ++i)
  {
    double exp = static_cast<double>(i - KERNAL_DIAMETER / 2);
    static const double s = static_cast<float>(KERNAL_DIAMETER / 2) / 2.0f;

    gGaussianWeights[i] = std::exp(-1.0f * (exp * exp) / (2 * s * s));
    sum += gGaussianWeights[i];
  }


  for (size_t i = 0; i < KERNAL_DIAMETER; ++i)
    gGaussianWeights[i] /= sum;
}

SceneView::~SceneView()
{
  SafeRelease(gSamplerState);
  SafeRelease(gShadowRasterizerState);
  ResourceLoader::Shutdown();
}

void SceneView::StartFrame()
{
  gBuffer->ClearRenderTarget();
  
  gLight1->ClearRenderTarget();
  gLight2->ClearRenderTarget();

  gBlurOutputBuffer->ClearRenderTarget();
  gBlurOutputBuffer2->ClearRenderTarget();

  gCamera->GenerateViewMatrix();
  gCamera->GenerateProjectionMatrix();

  Input();
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void SceneView::Update(float)
{
  ID3D11DeviceContext *devcon = D3D::GetInstance()->mDeviceContext;
  BEntityTransform trans_buffer;

    // SHADOW MAP PASS
    /////////////////////////////////////////////////////////////
  gShaderProgramShadowLight->Bind();

  ID3D11RasterizerState *rasterizer;
  devcon->RSGetState(&rasterizer);
  devcon->RSSetState(gShadowRasterizerState);

  for (auto &light : gLights)
  {
    light->GenerateViewMatrix();
    light->GenerateProjectionMatrix();
    
    trans_buffer.view = light->GetViewMatrix().Transpose();
    trans_buffer.projection = light->GetProjectionMatrix().Transpose();

    light->BindForRender();

    for (auto &entity : res::Entity::GetResources()) {
      trans_buffer.model = entity.second->GetTransform().Transpose();
      gEntityTransformBuffer->MapData(&trans_buffer);

      entity.second->mModel->Draw();
    }
  }

  devcon->RSSetState(rasterizer);


    // GEOMETRY PASS
    /////////////////////////////////////////////////////////////
  gBuffer->BindAsRenderTarget();

  trans_buffer.view = gCamera->GetViewMatrix().Transpose();
  trans_buffer.projection = gCamera->GetProjectionMatrix().Transpose();

  for (auto &entity : res::Entity::GetResources()) {
    trans_buffer.model = entity.second->GetTransform().Transpose();

      // You always have to transpose before putting it in a constant buffer,
      // BUUT since this is the inverse transpose...
    trans_buffer.trans_inv_model = (entity.second->GetTransform()).Invert();

    if (entity.second->mTexture.IsValid())
      entity.second->mTexture->Bind(0, res::Shader::Pixel);
    else
      res::Texture::ClearBinding(0, res::Shader::Pixel);
      
    entity.second->mShaderProgram->Bind();
    gEntityTransformBuffer->MapData(&trans_buffer);

    entity.second->mModel->Draw();
  }

  // Save old topology
  D3D11_PRIMITIVE_TOPOLOGY topology;
  devcon->IAGetPrimitiveTopology(&topology);
  devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // GLOBAL LIGHTING PASS
    /////////////////////////////////////////////////////////////
  gShaderProgramPhong->Bind();
  gBuffer->BindAsResourceView(0);

  BLight light_buffer;

  light_buffer.light_pos = Vec3(0.0f, 30.0f, 0.0f);
  light_buffer.camera_pos = gCamera->mPosition;

  gLightBuffer->MapData(&light_buffer);

  D3D::GetInstance()->BindRenderTarget();

  devcon->Draw(4, 0);

    // LOCAL LIGHTING PASS
    /////////////////////////////////////////////////////////////

  BBlurFilter blur_filter_buffer;
  {

    for (int i = 0; i < KERNAL_DIAMETER; ++i) {
      blur_filter_buffer.weights[i] = Vec4(static_cast<float>(gGaussianWeights[i]));
    }
    blur_filter_buffer.max_input_size = 1024;
    blur_filter_buffer.max_output_size = 1024;
  }

  gBlurFilterBuffer->MapData(&blur_filter_buffer);

  for (auto &light : gLights)
  {
      // BLUR SHADOW-MAP STEP
      //////////////////////
    light->BindForResource(0, true);
    gBlurOutputBuffer->BindAsUnorderedAccessView(0);

    gShaderProgramHorizontalBlur->Bind();

    devcon->Dispatch(1024 / 128, 1024, 1);
      ///////////////////
    gBlurOutputBuffer->BindAsResourceView(0, true);
    gBlurOutputBuffer2->BindAsUnorderedAccessView(0);

    gShaderProgramVerticalBlur->Bind();

    devcon->Dispatch(1024, 1024 / 128, 1);
      /////////////////////////

    gBlurOutputBuffer2->BindAsResourceView(4);
    gBuffer->BindAsResourceView(0);

    gShaderProgramLocalLight->Bind();

    D3D::GetInstance()->BindRenderTarget();
    
  //  light->BindForResource(4, false);
    gBlurOutputBuffer2->BindAsResourceView(4);
    light_buffer.light_pos = light->mPosition;

    light_buffer.light_vp = (light->GetViewMatrix() * light->GetProjectionMatrix()).Transpose();
    
    gLightBuffer->MapData(&light_buffer);
    devcon->Draw(4, 0);
  }




  //gLight1->BindForResource(0, false);
  ////gBlurOutputBuffer2->BindAsResourceView(0);

  //gShaderProgramDefault->Bind();
  //D3D::GetInstance()->BindRenderTarget();
  //devcon->Draw(4, 0);
  // Reset old topology
  devcon->IASetPrimitiveTopology(topology);
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


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


  //if (kb.Right == true)
  //  gTestEntity->mRotation += Vec3(0.0f, 0.1f, 0.0f);
  //if (kb.Up == true)
  //  gTestEntity->mRotation += Vec3(0.0f, 0.0f, 0.1f);

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
 