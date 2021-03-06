#pragma once

#include <cassert>
#include <cstdlib>
#include <cstddef>

#include <iostream>
#include <fstream>
#include <istream>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <random>
#include <ctime>

#include <comdef.h>
#include <d3d11.h>
#include <Windows.h>

#include "Utils/ErrorReporting.hpp"
#include "Utils/D3DSafeRelease.hpp"

#include "DirectXTK/SimpleMath.h"
#include "DirectXTK/Keyboard.h"
#include "DirectXTK/Mouse.h"

typedef DirectX::SimpleMath::Vector2 Vec2;
typedef DirectX::SimpleMath::Vector3 Vec3;
typedef DirectX::SimpleMath::Vector4 Vec4;
typedef DirectX::SimpleMath::Matrix Mat;
typedef DirectX::SimpleMath::Quaternion Quat;