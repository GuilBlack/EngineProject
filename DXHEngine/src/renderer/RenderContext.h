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

	void CreateFence(ID3D12Fence** fence)
	{
		ASSERT_HRESULT(m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence)));
	}

	void CreateSwapChain(ID3D12CommandQueue* commandQueue, DXGI_SWAP_CHAIN_DESC& swapChainDesc, IDXGISwapChain** swapChain);

	void CreateRTVHeapDescriptor(uint32_t numDescriptors, ID3D12DescriptorHeap** rtvHeap)
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = 
		{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			.NumDescriptors = numDescriptors,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		};
		ASSERT_HRESULT(m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(rtvHeap)));
	}

	void CreateDSVHeapDescriptor(uint32_t numDescriptors, ID3D12DescriptorHeap** dsvHeap)
	{
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc =
		{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
			.NumDescriptors = numDescriptors,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		};
		ASSERT_HRESULT(m_pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap)));
	}

	void CreateRTV(ID3D12Resource* rtv, CD3DX12_CPU_DESCRIPTOR_HANDLE& rtvHeapHandle)
	{
		m_pDevice->CreateRenderTargetView(rtv, nullptr, rtvHeapHandle);
	}

	void CreateDSV(
		ID3D12Resource** dsv, 
		D3D12_CPU_DESCRIPTOR_HANDLE& dsvHeapHandle,
		CD3DX12_HEAP_PROPERTIES& heapProps,
		D3D12_RESOURCE_DESC& depthStencilDesc, 
		D3D12_CLEAR_VALUE& optClear,
		D3D12_DEPTH_STENCIL_VIEW_DESC& dsvDesc);

	uint32_t GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeadType)
	{ 
		return m_pDevice->GetDescriptorHandleIncrementSize(descriptorHeadType);
	}

private:
	ID3D12Device* m_pDevice = nullptr;
	IDXGIFactory4* m_pDXGIFactory = nullptr;
};
}