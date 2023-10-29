#pragma once

namespace DXH
{
class BaseShader;
struct Geometry;

struct Mesh
{
	Geometry* Geo = nullptr;
	BaseShader* Shader = nullptr;

	uint32_t CBVIndex = -1;
};
}
