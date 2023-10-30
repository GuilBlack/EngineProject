#pragma once

namespace DXH
{
enum class ShaderProgramType;
enum class InputLayoutType;
class BaseShader;
struct Geometry;
struct Mesh;
struct Material;
enum class MaterialType;

class RendererResource
{
	friend class Renderer;
public:
	RendererResource() = default;
	~RendererResource();

	/// <summary>
	/// Singleton
	/// </summary>
	static RendererResource& GetInstance()
	{
		static RendererResource instance;
		return instance;
	}

	/// <summary>
	/// Initialize all basic resources
	/// </summary>
	void Init();

	/// <summary>
	/// Create a shader
	/// </summary>
	/// <param name="name">Name of the shader</param>
	/// <param name="vsFilePath">Path to the vertex shader</param>
	/// <param name="psFilePath">Path to the pixel shader</param>
	/// <param name="type">Type of the shader program</param>
	/// <param name="layout">Type of the input layout</param>
	void CreateShader(const std::string& name, const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout);

	void CreateMaterial(const std::string& materialName, MaterialType materialType, const std::string& shaderName);

	/// <summary>
	/// Create a Mesh
	/// </summary>
	/// <param name="shaderName">Name of the shader that will be used.</param>
	/// <param name="geometryName">Name of the geometry that will be used.</param>
	Mesh CreateMesh(const std::string& shaderName, const std::string& geometryName);

	/// <summary>
	/// Get a material
	/// </summary>
	/// <param name="materialName">Name of the material</param>
	/// <returns>Pointer to the material</returns>
	Material* GetMaterial(const std::string& materialName) { return m_Materials[materialName]; }

	Geometry* GetGeometry(const std::string& geometryName) { return m_Geometries[geometryName]; }

private:
	std::unordered_map<std::string, BaseShader*> m_Shaders;
	std::unordered_map<std::string, Geometry*> m_Geometries;
	std::unordered_map<std::string, Material*> m_Materials;

private:
	/// <summary>
	/// Create a cube geometry
	/// </summary>
	void CreateCube();

	/// <summary>
	/// Create a square geometry
	/// </summary>
	void CreateSquare();
};
}