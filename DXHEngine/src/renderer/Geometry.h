#pragma once
#include <limits>
#include "Util.h"
#include "Shader.h"

namespace DXH
{
struct SphereBoundingVolume;

/// <summary>
/// Geometry struct that contains all the data about a geometry on the CPU and the GPU
/// </summary>
struct Geometry
{
    Geometry() = default;
    Geometry(void* vertices, std::vector<uint16_t> indices, uint32_t vbByteSize, uint32_t vertexByteStride);
    virtual ~Geometry();

    ID3DBlob* VertexBufferCPU = nullptr;
    ID3DBlob* IndexBufferCPU = nullptr;

    ID3D12Resource* VertexBufferGPU = nullptr;
    ID3D12Resource* IndexBufferGPU = nullptr;

    uint32_t VertexByteStride = 0;
    uint32_t VertexBufferByteSize = 0;
    uint32_t IndexBufferByteSize = 0;
    DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;

    SphereBoundingVolume BoundingSphere;

    /// <summary>
    /// Returns the vertex buffer view
    /// </summary>
    virtual D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
    {
        D3D12_VERTEX_BUFFER_VIEW vbv = {};
        vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
        vbv.SizeInBytes = VertexBufferByteSize;
        vbv.StrideInBytes = VertexByteStride;

        return vbv;
    }

    /// <summary>
    /// Returns the index buffer view
    /// </summary>
    D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
    {
        D3D12_INDEX_BUFFER_VIEW ibv = {};
        ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
        ibv.Format = IndexFormat;
        ibv.SizeInBytes = IndexBufferByteSize;

        return ibv;
    }

    template<typename VertexType>
    static SphereBoundingVolume ComputeBoundingSphere(std::vector<VertexType> vertices)
    {
        using namespace DirectX;
        static constexpr float min = std::numeric_limits<float>::max();
        static constexpr float max = std::numeric_limits<float>::lowest();

        Vector3 minAABB = Vector3(min, min, min);
        Vector3 maxAABB = Vector3(max, max, max);

        for (uint32_t i = 0; i < vertices.size(); ++i)
        {
            XMFLOAT3 p = vertices[i].Position;

            minAABB.x = MIN(minAABB.x, p.x);
            minAABB.y = MIN(minAABB.y, p.y);
            minAABB.z = MIN(minAABB.z, p.z);

            maxAABB.x = MAX(maxAABB.x, p.x);
            maxAABB.y = MAX(maxAABB.y, p.y);
            maxAABB.z = MAX(maxAABB.z, p.z);
        }
        XMVECTOR center = XMVectorScale(XMVectorAdd(maxAABB.Load(), minAABB.Load()), 0.5f);
        float distCenterToMax = XMVectorGetX(XMVector3Length(maxAABB.Load() - center));
        float distCenterToMin = XMVectorGetX(XMVector3Length(minAABB.Load() - center));
        float radius = MAX(distCenterToMax, distCenterToMin);
        return SphereBoundingVolume(center, radius);
    }
};

struct NumberGeometry : public Geometry
{
    UploadBuffer<PosNormTexcoordVertex> VertexBuffer;

    NumberGeometry() = default;
    NumberGeometry(uint32_t numCharacters);
    ~NumberGeometry();

    virtual D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const override;
};
}
