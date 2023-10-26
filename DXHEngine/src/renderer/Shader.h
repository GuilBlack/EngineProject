#pragma once

namespace DXH
{
	enum class ShaderProgramType
	{
		SimpleShader,
		PhongShader
	};
	class BaseShader
	{
	public:
		BaseShader() = default;
		virtual ~BaseShader() = default;

		static BaseShader* Create(std::string vsFilePath, std::string psFilePath, ShaderProgramType type);

	private:
		ID3DBlob* m_pVS;
		ID3DBlob* m_pPS;
		ID3D12PipelineState* m_pPSO;
		ID3D12RootSignature* m_pRootSignature;
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;


	private:
		static ID3DBlob* LoadCompiledShader(std::string filepath);
	};

	class SimpleShader : public BaseShader
	{

	};

	class PhongShader : public BaseShader
	{

	};
}

