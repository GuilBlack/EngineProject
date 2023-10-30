#pragma once
#include "Shader.h"


namespace DXH
{
enum class MaterialType
{
	Simple,
	Lighting
};

struct Material
{
	uint32_t MaterialCBIndex = -1;
	MaterialType Type = MaterialType::Simple;
	BaseShader* Shader = nullptr;
};

struct LightingMaterial : public Material
{

};
}