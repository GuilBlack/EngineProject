#pragma once

namespace DXH
{
enum class ShaderProgramType;
enum class InputLayoutType;
class BaseShader;
struct Geometry;
struct Mesh;

class RendererResource
{
	friend class Renderer;
public:
	RendererResource() = default;
	~RendererResource();

	static RendererResource& GetInstance()
	{
		static RendererResource instance;
		return instance;
	}

	void Init();

	void CreateShader(const std::string& name, const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout);

	BaseShader* GetShader(const std::string& name) { return m_Shaders[name]; }


	Mesh CreateMesh(const std::string& shaderName, const std::string& geometryName);

private:
	std::unordered_map<std::string, BaseShader*> m_Shaders;
	std::unordered_map<std::string, Geometry*> m_Geometries;

private:
	void CreateCube();
	void CreateSquare();
};
}