#pragma once
#include "../../renderer/Shader.h"
#include "../Component.h"

namespace DXH
{
class BaseShader;
struct Geometry;
struct Material;

struct Mesh : public Component
{
	Geometry* Geo = nullptr;
	Material* Mat = nullptr;

	virtual void OnRelease() override
	{
		Geo = nullptr;
		Mat = nullptr;
	}

	virtual void OnGet() override
	{
	}

	Mesh()
	{
		CBVIndex = BaseShader::AddObjectCB();
	}

	uint32_t GetCBIndex() const
	{
		return CBVIndex;
	}

private:

	uint32_t CBVIndex = -1;
};
}