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

	virtual void OnDetach() override
	{
		Geo = nullptr;
		Mat = nullptr;
	}

	virtual void OnAssign() override
	{
		if (!isInitialized)
		{
			isInitialized = true;
			CBVIndex = BaseShader::AddObjectCB();
		}
	}

	uint32_t GetCBIndex() const
	{
		return CBVIndex;
	}

private:
	bool isInitialized = false;
	uint32_t CBVIndex = -1;
};
}