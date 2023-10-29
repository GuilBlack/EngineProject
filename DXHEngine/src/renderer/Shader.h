#pragma once
#include "ConstantBuffers.h"

namespace DXH
{
enum class ShaderProgramType
{
	SimpleShader,
};

enum class InputLayoutType
{
	Position,
	PositionColor,
	PositionTexcoord,
	PositionNormalColor,
	PositionNormalTexcoord
};

struct BasicVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

struct Geometry;
struct Transform;

class BaseShader
{
public:
	BaseShader();
	virtual ~BaseShader();

	static BaseShader* Create(const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout);

	virtual void Bind(ID3D12GraphicsCommandList* cl) 
	{
		cl->SetPipelineState(m_pPSO);
		cl->SetGraphicsRootSignature(m_pRootSignature);
		cl->SetGraphicsRootConstantBufferView(1, m_PassCB.GetResource()->GetGPUVirtualAddress());
	}
	
	virtual void Draw(Geometry* geometry, uint32_t objectCBIndex, Transform& transform, ID3D12GraphicsCommandList* cl);

	virtual void Unbind(ID3D12GraphicsCommandList* cl) {}

	uint32_t AddObjectCB() 
	{
		ObjectConstants objectCB;
		m_ObjectCB.push_back(UploadBuffer<ObjectConstants>());
		m_ObjectCB.back().Init(1, true);
		m_ObjectCB.back().CopyData(0, objectCB);
		return (uint32_t)m_ObjectCB.size() - 1;
	}
	void UpdatePassCB(PassConstants& passCB) { m_PassCB.CopyData(0, passCB); }
	void UpdateObjectCB(ObjectConstants& objectCB, uint32_t index) { m_ObjectCB[index].CopyData(0, objectCB); }

protected:
	ID3DBlob* m_pVS = nullptr;
	ID3DBlob* m_pPS = nullptr;
	
	ID3D12PipelineState* m_pPSO = nullptr;
	ID3D12RootSignature* m_pRootSignature = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

	UploadBuffer<PassConstants> m_PassCB;
	std::vector<UploadBuffer<ObjectConstants>> m_ObjectCB;
	ShaderProgramType m_Type;

protected:
	static ID3DBlob* LoadCompiledShader(const std::string& filepath);
	static std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(InputLayoutType layout);
	void BuildRootSignature(CD3DX12_ROOT_PARAMETER* rootParameters, uint32_t numParameters);
	void BuildPSO();
};

class SimpleShader : public BaseShader
{
public:
	SimpleShader();

	virtual void Bind(ID3D12GraphicsCommandList* cl) override;

	virtual void Unbind(ID3D12GraphicsCommandList* cl) override;
private:
};
}

