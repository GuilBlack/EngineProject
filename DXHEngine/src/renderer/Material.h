#pragma once
#include "DXHMaths.h"


namespace DXH
{
enum class MaterialType
{
    None,
    Simple,
    Lighting,
    TextureLighting,
    NumberUI
};

struct Material
{
    virtual ~Material() = default;

    uint32_t MaterialCBIndex = -1;
    MaterialType Type = MaterialType::None;
    BaseShader* Shader = nullptr;
};

struct SimpleLightingMaterial : public Material
{
    virtual ~SimpleLightingMaterial() = default;

    Color DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    Vector3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
    float Roughness = 0.25f;
};

struct TextureLightingMaterial : public SimpleLightingMaterial
{
    Texture* DiffuseTexture = nullptr;
};

struct NumberUIMaterial : public Material
{
    Texture* NumberAtlas = nullptr;
};
}
