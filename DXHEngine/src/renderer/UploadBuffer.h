#pragma once
#include "Renderer.h"

namespace DXH
{
template<typename BufferType>
class UploadBuffer
{
public:
	UploadBuffer(uint32_t elementCount, bool isConstantBuffer = true)
		: m_IsConstantBuffer(isConstantBuffer)
	{
		if (isConstantBuffer)
			m_ElementByteSize = UtilFunc::GetCBByteSize(sizeof(BufferType));
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

	UploadBuffer(const UploadBuffer& rhs) = delete;
	UploadBuffer& operator=(const UploadBuffer& rhs) = delete;

	~UploadBuffer()
	{
		if (m_UploadBuffer != nullptr)
			m_UploadBuffer->Unmap(0, nullptr);

		m_CPUData = nullptr;
	}

	ID3D12Resource* GetResource() const
	{
		return m_UploadBuffer.Get();
	}

	void CopyData(int elementIndex, const BufferType& data)
	{
		memcpy(&m_CPUData[elementIndex * m_ElementByteSize], &data, sizeof(BufferType));
	}

private:
	bool m_IsConstantBuffer;
	uint32_t m_ElementByteSize = 0;

	uint8_t* m_CPUData = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_UploadBuffer;
};
}