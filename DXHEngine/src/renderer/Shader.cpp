#include "Geometry.h"
#include "Renderer.h"
#include "../ecs/components/Transform.h"

#include "Shader.h"

#include "Util.h"

namespace DXH
{
std::vector<UploadBuffer<ObjectConstants>> BaseShader::s_ObjectCB;
//////////////////////////////////////////////////////////////////////////
// BaseShader ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

	BaseShader::BaseShader()
		: m_PassCB(1, true)
	{
		m_PassCB.CopyData(0, PassConstants());
	}

	BaseShader::~BaseShader()
{
	RELEASE_PTR(m_pVS);
	RELEASE_PTR(m_pPS);
	RELEASE_PTR(m_pPSO);
	RELEASE_PTR(m_pRootSignature);
}

BaseShader* BaseShader::Create(const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout)
{
	BaseShader* shader = nullptr;

	switch (type)
	{
	case ShaderProgramType::SimpleShader:
	{
		shader = new SimpleShader();
		break;
	}
	}
	assert(shader && "Wrong shader program type given!");

	shader->m_Type = type;
	shader->m_pVS = LoadCompiledShader(vsFilePath);
	shader->m_pPS = LoadCompiledShader(psFilePath);
	shader->m_InputLayout = CreateInputLayout(layout);

	shader->BuildPSO();

	return shader;
}

void BaseShader::Draw(Geometry* geometry, uint32_t objectCBIndex, Transform& transform, ID3D12GraphicsCommandList* cl)
{
	using namespace DirectX;
	ObjectConstants objectCB;
	XMStoreFloat4x4(&objectCB.World, XMMatrixTranspose(transform.GetModelMatrix()));
	UpdateObjectCB(objectCB, objectCBIndex);
	D3D12_VERTEX_BUFFER_VIEW vbv = geometry->VertexBufferView();
	D3D12_INDEX_BUFFER_VIEW ibv = geometry->IndexBufferView();

	cl->SetGraphicsRootConstantBufferView(0, s_ObjectCB[objectCBIndex].GetResource()->GetGPUVirtualAddress());
	cl->IASetVertexBuffers(0, 1, &vbv);
	cl->IASetIndexBuffer(&ibv);
	cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cl->DrawIndexedInstanced(geometry->IndexBufferByteSize / sizeof(uint16_t), 1, 0, 0, 0);
}

uint32_t BaseShader::AddObjectCB()
{
	ObjectConstants objectCB;
	s_ObjectCB.push_back(UploadBuffer<ObjectConstants>());
	s_ObjectCB.back().Init(1, true);
	s_ObjectCB.back().CopyData(0, objectCB);
	return (uint32_t)s_ObjectCB.size() - 1;
}

ID3DBlob* BaseShader::LoadCompiledShader(const std::string& filepath)
{
	std::ifstream fin(filepath, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	uint32_t size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);

	ID3DBlob* bin;
	ASSERT_HRESULT(D3DCreateBlob(size, &bin));
	fin.read((char*)bin->GetBufferPointer(), size);

	fin.close();

	return bin;
}

std::vector<D3D12_INPUT_ELEMENT_DESC> BaseShader::CreateInputLayout(InputLayoutType layout)
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

	switch (layout)
	{
	case InputLayoutType::Position:
		inputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		break;
	case InputLayoutType::PositionColor:
		inputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		break;
	case InputLayoutType::PositionNormalColor:
		inputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		break;
	default:
		assert(false && "this layout isn't implemented yet!");
	}
	return inputLayout;
}

void BaseShader::BuildRootSignature(CD3DX12_ROOT_PARAMETER* rootParameters, uint32_t numParameters)
{
	ID3DBlob* serializedRootSignature = nullptr;
	ID3DBlob* error = nullptr;

	HRESULT hr = S_OK;

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init(
		numParameters, rootParameters,
		0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &error);

	ASSERT_HRESULT(hr);

	Renderer::GetRenderContext()->CreateRootSignature(
		serializedRootSignature,
		&m_pRootSignature
	);

	RELEASE_PTR(serializedRootSignature);
	RELEASE_PTR(error);
}

void BaseShader::BuildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc =
	{
		.pRootSignature = m_pRootSignature,
		.VS = 
		{
			reinterpret_cast<BYTE*>(m_pVS->GetBufferPointer()),
			m_pVS->GetBufferSize()
		},
		.PS = 
		{
			reinterpret_cast<BYTE*>(m_pPS->GetBufferPointer()),
			m_pPS->GetBufferSize()
		},
		.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT),
		.SampleMask = UINT_MAX,
		.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
		.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT),
		.InputLayout = { m_InputLayout.data(), (uint32_t)m_InputLayout.size() },
		.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		.NumRenderTargets = 1,
		.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.SampleDesc = { 1, 0 },
		.NodeMask = 0
	};
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	Renderer::GetRenderContext()->CreatePSO(psoDesc, &m_pPSO);
}

//////////////////////////////////////////////////////////////////////////
// SimpleShader //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SimpleShader::SimpleShader()
	: BaseShader()
{
	m_Type = ShaderProgramType::SimpleShader;
	CD3DX12_ROOT_PARAMETER rootParameters[2];

	rootParameters[0].InitAsConstantBufferView(0); // b0
	rootParameters[1].InitAsConstantBufferView(1); // b1

	BuildRootSignature(rootParameters, 2);
}

void SimpleShader::Bind(ID3D12GraphicsCommandList* cl)
{
	BaseShader::Bind(cl);
}

BasicPhongShader::BasicPhongShader()
{
	m_Type = ShaderProgramType::BasicPhongShader;

	CD3DX12_ROOT_PARAMETER rootParameters[3];

	rootParameters[0].InitAsConstantBufferView(0); // b0
	rootParameters[1].InitAsConstantBufferView(1); // b1
	rootParameters[2].InitAsConstantBufferView(2); // b2

	BuildRootSignature(rootParameters, 3);
}

void BasicPhongShader::Bind(ID3D12GraphicsCommandList* cl)
{
	BaseShader::Bind(cl);
}

}

