#pragma once

namespace DXH
{
class BaseShader;

struct Material
{
	BaseShader* shader;
	uint32_t MaterialCBIndex;
};

}