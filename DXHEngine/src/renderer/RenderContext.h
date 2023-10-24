#pragma once

namespace DXH
{
class RenderContext
{
public:
	RenderContext();
	~RenderContext();

	void Init();
	void Destroy();

	void CreateCommandObjects(ID3D12CommandQueue** commandQueue, ID3D12CommandAllocator** commandAllocator, ID3D12GraphicsCommandList** commandList);

	void CreateSwapChain(ID3D12CommandQueue* commandQueue, DXGI_SWAP_CHAIN_DESC& swapChainDesc, IDXGISwapChain* swapChain);

	void CreateRTVHeapDescriptor(D3D12_DESCRIPTOR_HEAP_DESC& rtvHeapDesc, ID3D12DescriptorHeap** rtvHeap)
	{
		ASSERT_HRESULT(m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(rtvHeap)));
	}

	void CreateDSVHeapDescriptor(D3D12_DESCRIPTOR_HEAP_DESC& dsvHeapDesc, ID3D12DescriptorHeap** dsvHeap)
	{
		ASSERT_HRESULT(m_pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap)));
	}

	uint32_t GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeadType)
	{ 
		return m_pDevice->GetDescriptorHandleIncrementSize(descriptorHeadType);
	}
private:

private:
	ID3D12Device* m_pDevice = nullptr;
	IDXGIFactory4* m_pDXGIFactory = nullptr;
	ID3D12Fence* m_pFence = nullptr;
};
}