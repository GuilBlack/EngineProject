#pragma once
#include "Renderer.h"
#include "Util.h"

namespace DXH
{
struct PassConstants
{
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Proj;
	DirectX::XMFLOAT4X4 ViewProj;
	DirectX::XMFLOAT3 EyePosW;
	float NearZ;
	float FarZ;
	float TotalTime;
	DirectX::XMFLOAT2 RenderTargetSize;
	float DeltaTime;
};

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 World = DirectX::XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
};

template<typename BufferType>
class UploadBuffer
{
public:
	UploadBuffer() = default;
	UploadBuffer(uint32_t elementCount, bool isConstantBuffer = true)
		: m_IsConstantBuffer(isConstantBuffer)
	{
		Init(elementCount, isConstantBuffer);
	}

	UploadBuffer(const UploadBuffer& rhs) = default;
	UploadBuffer& operator=(const UploadBuffer& rhs) = delete;

	void Init(uint32_t elementCount, bool isConstantBuffer = true)
	{
		if (isConstantBuffer)
			m_ElementByteSize = GetCBByteSize(sizeof(BufferType));
		else
			m_ElementByteSize = sizeof(BufferType);

		D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize * elementCount);

		Renderer::GetInstance().GetRenderContext()->CreateResource(
			&m_UploadBuffer,
			heapProperties,
			resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ
		);

		ASSERT_HRESULT(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_CPUData)));
	}

	~UploadBuffer()
	{
		if (m_UploadBuffer != nullptr)
		{
			m_UploadBuffer->Unmap(0, nullptr);
		}
		m_CPUData = nullptr;
	}

	ID3D12Resource* GetResource() const
	{
		return m_UploadBuffer;
	}

	void CopyData(int elementIndex, const BufferType& data)
	{
		memcpy(&m_CPUData[elementIndex * m_ElementByteSize], &data, sizeof(BufferType));
	}

private:
	bool m_IsConstantBuffer = true;
	uint32_t m_ElementByteSize = 0;

	uint8_t* m_CPUData = nullptr;
	ID3D12Resource* m_UploadBuffer = nullptr;
};
}