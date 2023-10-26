#include "Shader.h"

namespace DXH
{
	BaseShader* BaseShader::Create(std::string vsFilePath, std::string psFilePath, ShaderProgramType type)
	{
		BaseShader* shader = nullptr;
		switch (type)
		{
		case ShaderProgramType::SimpleShader:
		{
			shader = new SimpleShader();
			break;
		}
		case ShaderProgramType::PhongShader:
		{
			shader = new PhongShader();
			break;
		}
		default:
			assert(false && "Shader type not supported");
		}

		shader->m_pVS = LoadCompiledShader(vsFilePath);
		shader->m_pPS = LoadCompiledShader(psFilePath);


		return shader;
	}


ID3DBlob* BaseShader::LoadCompiledShader(std::string filepath)
{
	std::ifstream fin(filepath, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	std::ifstream::pos_type size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);

	ID3DBlob* bin;
	ASSERT_HRESULT(D3DCreateBlob(size, &bin));
	fin.read((char*)bin->GetBufferPointer(), size);

	fin.close();

	return bin;
}
}

