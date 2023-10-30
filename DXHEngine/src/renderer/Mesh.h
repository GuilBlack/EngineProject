#pragma once

namespace DXH
{
class BaseShader;
struct Geometry;
struct Material;

struct Mesh
{
	Geometry* Geo = nullptr;
	Material* Mat = nullptr;

	uint32_t CBVIndex = -1;
};
}