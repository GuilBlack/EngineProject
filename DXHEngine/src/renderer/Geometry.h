#pragma once

namespace DXH
{
struct Geometry
{
	Geometry() = default;
	Geometry(const std::string& name, void* vertices, std::vector<uint16_t> indices, uint32_t vbByteSize, uint32_t vertexByteStride);
	~Geometry();

	std::string Name = "";

	ID3DBlob* VertexBufferCPU = nullptr;
	ID3DBlob* IndexBufferCPU = nullptr;

	ID3D12Resource* VertexBufferGPU = nullptr;
	ID3D12Resource* IndexBufferGPU = nullptr;

	uint32_t VertexByteStride = 0;
	uint32_t VertexBufferByteSize = 0;
	uint32_t IndexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv = {};
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.SizeInBytes = VertexBufferByteSize;
		vbv.StrideInBytes = VertexByteStride;

		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
	{
		D3D12_INDEX_BUFFER_VIEW ibv = {};
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}
};

}