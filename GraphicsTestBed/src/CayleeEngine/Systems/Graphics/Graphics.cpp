#include "precompiled.hpp"
#include "Graphics.hpp"

#include "Singletons/Window/Window.hpp"
#include "Singletons/D3D/D3D.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace CayleeEngine::sys
{
Graphics::Graphics()
{
  Window::Initialize();
  D3D::Initialize();

  ImGui::SetCurrentContext(ImGui::CreateContext());
  ImGuiIO &io = ImGui::GetIO();

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui_ImplWin32_Init(Window::GetInstance()->GetWindowHandle());
  ImGui_ImplDX11_Init(D3D::GetInstance()->mDevice, D3D::GetInstance()->mDeviceContext);
}

Graphics::~Graphics()
{
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  D3D::Shutdown();
  Window::Shutdown();
}

void Graphics::StartFrame()
{
  D3D::GetInstance()->ClearRenderTarget();
  D3D::GetInstance()->BindRenderTarget();
  D3D::GetInstance()->mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void Graphics::Update(float)
{
}
void Graphics::EndFrame()
{
  ImGui::EndFrame();

  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  
  D3D::GetInstance()->mSwapChain->Present(1, 0);
}
}