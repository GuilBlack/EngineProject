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
    Matrix View;
    Matrix Proj;
    Matrix ViewProj;
    Vector3 EyePosW;
    float NearZ;
    float FarZ;
    float TotalTime;
    Vector2 RenderTargetSize;
    float DeltaTime;
    float AmbientIntensity;
    float SunIntensity;
    float Padding;
    Vector3 SunDirection;
    float Padding1;
    Vector3 SunColor;
};

struct LightsConstants
{

};

/// <summary>
/// Constant buffer for the object.
/// </summary>
struct ObjectConstants
{
    Matrix World = Matrix::Identity;
};

/// <summary>
/// Constant buffer for the simple lighting material.
/// </summary>
struct LightingMaterialConstants
{
    Vector4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    Vector3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
    float Roughness = 0.25f;
};
}
