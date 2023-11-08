#include "UploadBuffer.h"
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
    if (VertexBufferCPU)
        RELEASE_PTR(VertexBufferCPU);
    if (IndexBufferCPU)
        RELEASE_PTR(IndexBufferCPU);

    if (VertexBufferGPU)
        RELEASE_PTR(VertexBufferGPU);
    if (IndexBufferGPU)
        RELEASE_PTR(IndexBufferGPU);
}

//////////////////////////////////////////////////////////////////////////
// NumberGeometry ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

NumberGeometry::NumberGeometry(uint32_t numCharacters)
    : VertexBuffer()
{
    std::vector<PosNormTexcoordVertex> vertices;
    vertices.resize(4 * numCharacters);
    std::vector<std::uint16_t> indices;
    indices.resize(6 * numCharacters);
    float uvStride = .1f;
    for (uint32_t i = 0; i < numCharacters; i++)
    {
        uint32_t indexOffset = i * 6;
        uint32_t vertexOffset = i * 4;

        indices[indexOffset+0] = vertexOffset + 0;
        indices[indexOffset+1] = vertexOffset + 1;
        indices[indexOffset+2] = vertexOffset + 2;
        indices[indexOffset+3] = vertexOffset + 1;
        indices[indexOffset+4] = vertexOffset + 3;
        indices[indexOffset+5] = vertexOffset + 2;

        vertices[vertexOffset+0] = { Vector3(i*.78f+0.0f, 0.0f, 0.0f), Vector3::Backward, Vector2(0.0f, 0.0f) };
        vertices[vertexOffset+1] = { Vector3(i*.78f+.78f, 0.0f, 0.0f), Vector3::Backward, Vector2(uvStride, 0.0f) };
        vertices[vertexOffset+2] = { Vector3(i*.78f+0.0f, -1.0f, 0.0f), Vector3::Backward, Vector2(0.0f, 1.0f) };
        vertices[vertexOffset+3] = { Vector3(i*.78f+.78f, -1.0f, 0.0f), Vector3::Backward, Vector2(uvStride, 1.0f) };
    }

    vertices.shrink_to_fit();
    indices.shrink_to_fit();

    VertexBufferByteSize = (uint32_t)vertices.size() * sizeof(PosNormTexcoordVertex);
    VertexByteStride = sizeof(PosNormTexcoordVertex);
    IndexBufferByteSize = (uint32_t)indices.size() * sizeof(uint16_t);

    ASSERT_HRESULT(D3DCreateBlob(VertexBufferByteSize, &VertexBufferCPU));
    CopyMemory(VertexBufferCPU->GetBufferPointer(), vertices.data(), VertexBufferByteSize);

    ASSERT_HRESULT(D3DCreateBlob(IndexBufferByteSize, &IndexBufferCPU));
    CopyMemory(IndexBufferCPU->GetBufferPointer(), indices.data(), IndexBufferByteSize);

    VertexBuffer.Init((uint32_t)vertices.size(), false);
    for (uint32_t i = 0; i < vertices.size(); i++)
    {
        VertexBuffer.CopyData(i, vertices[i]);
    }
    IndexBufferGPU = Renderer::GetInstance().CreateDefaultBuffer((void*)indices.data(), IndexBufferByteSize);
}

NumberGeometry::~NumberGeometry()
{
    VertexBuffer.Destroy();
}

D3D12_VERTEX_BUFFER_VIEW NumberGeometry::VertexBufferView() const
{
    D3D12_VERTEX_BUFFER_VIEW vbv = {};
    vbv.BufferLocation = VertexBuffer.GetResource()->GetGPUVirtualAddress();
    vbv.SizeInBytes = VertexBufferByteSize;
    vbv.StrideInBytes = VertexByteStride;

    return vbv;
}

}
