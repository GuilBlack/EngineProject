#pragma once
#define NOMINMAX
// Windows kit
#include <Windows.h>
#include <wrl.h>
#include <comdef.h>

#include <cassert>
#include <limits>

#include <vector>
#include <unordered_map>

#include <fstream>
#include <sstream>

// DirectX
#include <vendor/d3dx12.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

// Our stuff
#include "src/core/Defines.h"
