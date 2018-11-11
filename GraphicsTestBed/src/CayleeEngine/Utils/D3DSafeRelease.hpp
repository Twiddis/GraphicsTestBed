#pragma once

template <typename T>
void constexpr SafeRelease(T* &rhs)
{
  if (rhs)
    rhs->Release();

  rhs = nullptr;
}