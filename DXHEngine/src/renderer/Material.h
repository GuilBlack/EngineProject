#pragma once
#include "Shader.h"
#include "DXHMaths.h"


namespace DXH
{
enum class MaterialType
{
    None,
    Simple,
    Lighting
};

struct Material
{
    uint32_t MaterialCBIndex = -1;
    MaterialType Type = MaterialType::None;
    BaseShader* Shader = nullptr;

    virtual ~Material() = default;
};

struct SimplePhongMaterial : public Material
{
    Vector4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    Vector3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
    float Roughness = 0.25f;
};
}
