#pragma once
#include "Geometry.h"
#include "Shader.h"

namespace DXH
{
	struct Mesh
	{
		Geometry* Geo;

		uint32_t CBVIndex = -1;

		BaseShader* Shader = nullptr;
	};
}
