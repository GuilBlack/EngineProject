#pragma once
#include "Renderer.h"

namespace DXH
{
/// <summary>
/// upload buffer for the constants buffers.
/// </summary>
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

    /// <summary>
    /// Create the upload buffer for a constants buffer of type BufferType with elementCount elements.
    /// </summary>
    /// <param name="elementCount">Number of elements in the buffer.</param>
    /// <param name="isConstantBuffer">Is this buffer a constant buffer?</param>
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

    void Destroy()
    {
        if (m_UploadBuffer != nullptr)
        {
            m_UploadBuffer->Unmap(0, nullptr);
            m_UploadBuffer->Release();
        }

        m_CPUData = nullptr;
    }

    ~UploadBuffer()
    {}

    /// <summary>
    /// Get the constant buffer on the GPU.
    /// </summary>
    /// <returns>Pointer to the constant buffer on the GPU.</returns>
    ID3D12Resource* GetResource() const
    {
        return m_UploadBuffer;
    }

    void CopyData(uint32_t elementIndex, const BufferType& data)
    {
        memcpy(&m_CPUData[elementIndex * m_ElementByteSize], &data, sizeof(BufferType));
    }

    /// <summary>
    /// Copy the data to the constant buffer on the CPU and maps it on the GPU.
    /// </summary>
    /// <param name="elementIndex">Index of the element to copy.</param>
    template<typename ElementType>
    void CopyData(uint32_t elementIndexByte, const ElementType& data, size_t dataSizeByte = sizeof(ElementType))
    {
        memcpy(&m_CPUData[elementIndexByte], &data, dataSizeByte);
    }

private:
    bool m_IsConstantBuffer = true;
    uint32_t m_ElementByteSize = 0;

    uint8_t* m_CPUData = nullptr;
    ID3D12Resource* m_UploadBuffer = nullptr;
};
}
