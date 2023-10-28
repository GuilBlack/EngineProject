#pragma once

namespace DXH
{
enum class ShaderProgramType;
enum class InputLayoutType;
class BaseShader;

class RendererResource
{
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

private:
	std::unordered_map<std::string, BaseShader*> m_Shaders;
};
}

