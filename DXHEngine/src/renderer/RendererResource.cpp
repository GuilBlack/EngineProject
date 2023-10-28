#include "Shader.h"
#include "Mesh.h"

#include "RendererResource.h"

DXH::RendererResource::~RendererResource()
{
	for (auto[_, shader] : m_Shaders)
		DELETE_PTR(shader);
}

void DXH::RendererResource::Init()
{
	CreateShader("SimpleShader", "../DXHEngine/res/shaders/color_vs.cso", "../DXHEngine/res/shaders/color_ps.cso", ShaderProgramType::SimpleShader, InputLayoutType::PositionColor);
}

void DXH::RendererResource::CreateShader(const std::string& name, const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout)
{
	m_Shaders[name] = BaseShader::Create(vsFilePath, psFilePath, type, layout);
}
