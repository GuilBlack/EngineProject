#include "RendererResource.h"
#include "Geometry.h"
#include "Shader.h"
#include "../ecs/components/Render.h"
#include "Material.h"

namespace DXH
{
RendererResource::~RendererResource()
{
	for (auto[_, shader] : m_Shaders)
		DELETE_PTR(shader);
	for (auto[_, geometry] : m_Geometries)
		DELETE_PTR(geometry);
	for (auto[_, material] : m_Materials)
		DELETE_PTR(material);
}

void RendererResource::Init()
{
	CreateShader("SimpleShader", "../DXHEngine/res/shaders/color_vs.cso", "../DXHEngine/res/shaders/color_ps.cso", ShaderProgramType::SimpleShader, InputLayoutType::PositionColor);
	CreateMaterial("SimpleMaterial", MaterialType::Simple, "SimpleShader");
	CreateCube();
	CreateSquare();
	CreateSphere();
}

void RendererResource::CreateShader(const std::string& name, const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout)
{
	if (m_Shaders.contains(name))
		return;
	m_Shaders[name] = BaseShader::Create(vsFilePath, psFilePath, type, layout);
}

void RendererResource::CreateMaterial(const std::string& materialName, MaterialType materialType, const std::string& shaderName)
{
	if (m_Materials.contains(materialName))
		return;
	if (!m_Shaders.contains(shaderName))
	{
		// TODO: Add error handling
		assert(false && "Shader not found");
	}
	switch (materialType)
	{
	case MaterialType::Simple:
	{
		if (m_Shaders[shaderName]->GetType() != ShaderProgramType::SimpleShader) 
		{ // TODO: Add error handling
			assert(false && "Shader type mismatch");
			return;
		}
		Material* material = new Material();
		material->Shader = m_Shaders[shaderName];
		material->Type = MaterialType::Simple;
		m_Materials[materialName] = material;
		break;
	}
	default:
		// TODO: Add error handling
		assert(false && "Material type not supported");
		break;
	}
}

//Mesh RendererResource::CreateMesh(const std::string& materialName, const std::string& geometryName)
//{
//	if (m_Materials.contains(materialName) && m_Geometries.contains(geometryName))
//	{
//		Mesh mesh;
//
//		mesh.CBVIndex = m_Materials[materialName]->Shader->AddObjectCB();
//		mesh.Geo = m_Geometries[geometryName];
//		mesh.Mat = m_Materials[materialName];
//
//		return mesh;
//	}
//	// TODO: Add error handling
//	assert(false && "Material or Geometry not found");
//	return Mesh();
//}

void RendererResource::CreateCube()
{
	using namespace DirectX;

	if (m_Geometries.contains("Cube"))
		return;
	std::vector<BasicVertex> vertices =
	{
		BasicVertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
		BasicVertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
		BasicVertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Yellow) }),
		BasicVertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		BasicVertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
		BasicVertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }),
		BasicVertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::White) }),
		BasicVertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Cyan) })
	};
	vertices.shrink_to_fit();


	std::vector<std::uint16_t> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	indices.shrink_to_fit();

	uint32_t vbByteSize = (uint32_t)vertices.size() * sizeof(BasicVertex);
	uint32_t vertexByteStride = sizeof(BasicVertex);

	m_Geometries["Cube"] = new Geometry(vertices.data(), indices, vbByteSize, vertexByteStride);
}
void RendererResource::CreateSquare()
{
	using namespace DirectX;

	if (m_Geometries.contains("Square"))
		return;
	std::vector<BasicVertex> vertices =
	{
		BasicVertex({ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Black) }),
		BasicVertex({ XMFLOAT3(-1.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Red) }),
		BasicVertex({ XMFLOAT3(+1.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Yellow) }),
		BasicVertex({ XMFLOAT3(+1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Green) }),
	};
	vertices.shrink_to_fit();

	std::vector<std::uint16_t> indices =
	{
		0, 1, 2,
		0, 2, 3,
	};

	indices.shrink_to_fit();

	uint32_t vbByteSize = (uint32_t)vertices.size() * sizeof(BasicVertex);
	uint32_t vertexByteStride = sizeof(BasicVertex);

	m_Geometries["Square"] = new Geometry(vertices.data(), indices, vbByteSize, vertexByteStride);
}
void RendererResource::CreateSphere()
{
	using namespace DirectX;
	float radius = 1.f;
	uint32_t latitude = 32;
	uint32_t longitude = 32;

	uint32_t numVertices = (latitude + 1) * (longitude * 2);
	uint32_t numIndices = 2 * 3 * longitude + 2 * 3 * (latitude - 1) * longitude;

	std::vector<BasicVertex> vertices;
	vertices.resize(numVertices);
	std::vector<std::uint16_t> indices;
	indices.resize(numIndices);

	float latitudeStep = XM_PI / latitude;
	float longitudeStep = XM_2PI / longitude;

	uint32_t count = 0;

	// top vertex
	for (uint32_t i = 1; i <= longitude; ++i)
	{
		vertices[count].Position = XMFLOAT3(0.f, radius, 0.f);
		vertices[count].Color = XMFLOAT4(Colors::White);
		++count;
	}

	// middle vertices
	for (uint32_t i = 1; i < latitude + 1; ++i)
	{
		float pLat = (float)i * latitudeStep;
		for (uint32_t j = 0; j < longitude + 1; ++j)
		{
			float pLong = (float)j * longitudeStep;
			vertices[count].Position = XMFLOAT3(
				radius * sinf(pLat) * cosf(pLong),
				radius * cosf(pLat),
				radius * sinf(pLat) * sinf(pLong));
			vertices[count].Color = XMFLOAT4(Colors::White);
			++count;
		}
	}

	// bottom vertex
	for (uint32_t i = 1; i <= longitude; ++i)
	{
		vertices[count].Position = XMFLOAT3(0.f, -radius, 0.f);
		vertices[count].Color = XMFLOAT4(Colors::White);
		++count;
	}

	count = 0;

	for (uint32_t i = 0; i < longitude; ++i)
	{
		indices[count++] = i;
		indices[count++] = (longitude-1) + i + 2;
		indices[count++] = (longitude - 1) + i + 1;
	}

	for (uint32_t i = 0; i < latitude - 1; ++i)
	{
		for (uint32_t j = 0; j < longitude; ++j)
		{
			uint32_t index[4] = {
				longitude + i * (longitude + 1) + j,
				longitude + i * (longitude + 1) + (j + 1),
				longitude + (i + 1) * (longitude + 1) + (j + 1),
				longitude + (i + 1) * (longitude + 1) + j
			};

			indices[count++] = index[0];
			indices[count++] = index[1];
			indices[count++] = index[2];

			indices[count++] = index[0];
			indices[count++] = index[2];
			indices[count++] = index[3];
		}
	}

	const uint32_t southPoleIndex = numVertices - longitude;
	for (uint32_t i = 0; i < longitude; ++i)
	{
		indices[count++] = southPoleIndex + i;
		indices[count++] = southPoleIndex - (longitude + 1) + i;
		indices[count++] = southPoleIndex - (longitude + 1) + i + 1;
	}

	uint32_t vbByteSize = (uint32_t)vertices.size() * sizeof(BasicVertex);
	uint32_t vertexByteStride = sizeof(BasicVertex);

	m_Geometries["Sphere"] = new Geometry(vertices.data(), indices, vbByteSize, vertexByteStride);
}
}