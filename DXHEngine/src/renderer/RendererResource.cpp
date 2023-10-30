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
}

void RendererResource::Init()
{
	CreateShader("SimpleShader", "../DXHEngine/res/shaders/color_vs.cso", "../DXHEngine/res/shaders/color_ps.cso", ShaderProgramType::SimpleShader, InputLayoutType::PositionColor);
	CreateMaterial("SimpleMaterial", MaterialType::Simple, "SimpleShader");
	CreateCube();
	CreateSquare();
}

void RendererResource::CreateShader(const std::string& name, const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout)
{
	m_Shaders[name] = BaseShader::Create(vsFilePath, psFilePath, type, layout);
}

void RendererResource::CreateMaterial(const std::string& materialName, MaterialType materialType, const std::string& shaderName)
{
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
}
