#pragma once
#include "Util.h"
#include "DXHMaths.h"

namespace DXH
{
/// <summary>
/// Constant buffer for the pass.
/// </summary>
struct PassConstants
{
    DirectX::XMFLOAT4X4 View;
    DirectX::XMFLOAT4X4 Proj;
    DirectX::XMFLOAT4X4 ViewProj;
    DirectX::XMFLOAT3 EyePosW;
    float NearZ;
    float FarZ;
    float TotalTime;
    DirectX::XMFLOAT2 RenderTargetSize;
    float DeltaTime;
    DirectX::XMFLOAT3 SunDirection;
    DirectX::XMFLOAT3 SunColor;
    DirectX::XMFLOAT3 AmbientColor;
    float AmbientIntensity;
    float SunIntensity;
};

/// <summary>
/// Constant buffer for the object.
/// </summary>
struct ObjectConstants
{
    DirectX::XMFLOAT4X4 World = DirectX::XMFLOAT4X4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
};

/// <summary>
/// Constant buffer for the phong lighting material.
/// </summary>
struct PhongMaterialConstants
{
    DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
    float Roughness = 0.25f;
};
}