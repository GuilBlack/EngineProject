#include "Geometry.h"
#include "Renderer.h"

namespace DXH
{

Geometry::Geometry(void* vertices, std::vector<uint16_t> indices, uint32_t vbByteSize, uint32_t vertexByteStride)
{
    VertexByteStride = vertexByteStride;
    VertexBufferByteSize = vbByteSize;
    IndexBufferByteSize = (uint32_t)indices.size() * sizeof(uint16_t);

    ASSERT_HRESULT(D3DCreateBlob(VertexBufferByteSize, &VertexBufferCPU));
    CopyMemory(VertexBufferCPU->GetBufferPointer(), vertices, VertexBufferByteSize);

    ASSERT_HRESULT(D3DCreateBlob(IndexBufferByteSize, &IndexBufferCPU));
    CopyMemory(IndexBufferCPU->GetBufferPointer(), indices.data(), IndexBufferByteSize);

    VertexBufferGPU = Renderer::GetInstance().CreateDefaultBuffer(vertices, VertexBufferByteSize);
    IndexBufferGPU = Renderer::GetInstance().CreateDefaultBuffer((void*)indices.data(), IndexBufferByteSize);
}

Geometry::~Geometry()
{
    RELEASE_PTR(VertexBufferCPU);
    RELEASE_PTR(IndexBufferCPU);
    RELEASE_PTR(VertexBufferGPU);
    RELEASE_PTR(IndexBufferGPU);
}

SphereBoundingVolume Geometry::ComputeBoundingSphere(std::vector<BasicVertex> vertices)
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
    XMVECTOR center = XMVectorScale(maxAABB.Load() + minAABB.Load(), 0.5f);
    float distCenterToMax = XMVectorGetX(XMVector3Length(maxAABB.Load() - center));
    float distCenterToMin = XMVectorGetX(XMVector3Length(minAABB.Load() - center));
    //XMVectorGetX(XMVector3Length(maxAABB.Load() - minAABB.Load())
    float radius = MAX(distCenterToMax, distCenterToMin);
    return SphereBoundingVolume(center, radius);
}

}
