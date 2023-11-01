#pragma once

namespace DXH
{
/// <summary>
/// Geometry struct that contains all the data about a geometry on the CPU and the GPU
/// </summary>
struct Geometry
{
	Geometry() = default;
	Geometry(void* vertices, std::vector<uint16_t> indices, uint32_t vbByteSize, uint32_t vertexByteStride);
	~Geometry();

	ID3DBlob* VertexBufferCPU = nullptr;
	ID3DBlob* IndexBufferCPU = nullptr;

	ID3D12Resource* VertexBufferGPU = nullptr;
	ID3D12Resource* IndexBufferGPU = nullptr;

	uint32_t VertexByteStride = 0;
	uint32_t VertexBufferByteSize = 0;
	uint32_t IndexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;

	/// <summary>
	/// Returns the vertex buffer view
	/// </summary>
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
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
};

}