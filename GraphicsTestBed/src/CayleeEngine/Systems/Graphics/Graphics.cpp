#include "precompiled.hpp"
#include "Graphics.hpp"

#include "Singletons/Window/Window.hpp"
#include "Singletons/D3D/D3D.hpp"

namespace CayleeEngine::sys
{
Graphics::Graphics()
{
  Window::Initialize();
  D3D::Initialize();
}

Graphics::~Graphics()
{
  D3D::Shutdown();
  Window::Shutdown();
}

void Graphics::Update(float)
{
  D3D::GetInstance()->mSwapChain->Present(0, 0);
}
}