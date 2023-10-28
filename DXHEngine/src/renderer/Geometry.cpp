#include "Geometry.h"
#include "Renderer.h"

DXH::Geometry::Geometry(const std::string& name, void* vertices, std::vector<uint16_t> indices, uint32_t vbByteSize, uint32_t vertexByteStride)
	: Name(name)
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

DXH::Geometry::~Geometry()
{
	RELEASE_PTR(VertexBufferCPU);
	RELEASE_PTR(IndexBufferCPU);
	RELEASE_PTR(VertexBufferGPU);
	RELEASE_PTR(IndexBufferGPU);
}
