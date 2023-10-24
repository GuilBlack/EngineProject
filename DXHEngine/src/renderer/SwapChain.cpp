#include "SwapChain.h"

namespace DXH
{

void SwapChain::Init(Window* window, RenderContext* context, ID3D12CommandQueue* commandQueue)
{
	// Init directx12 swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc =
	{
		.BufferDesc =
		{
			.Width = (uint32_t)window->GetWidth(),
			.Height = (uint32_t)window->GetHeight(),
			.RefreshRate = { 60, 1 },
			.Format = m_BackBufferFormat,
			.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			.Scaling = DXGI_MODE_SCALING_UNSPECIFIED
		},
		.SampleDesc =
		{
			.Count = 1,
			.Quality = 0
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = m_RTVBufferCount,
		.OutputWindow = window->GetWindowHandle(),
		.Windowed = true,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	context->CreateSwapChain(commandQueue, swapChainDesc, m_pSwapChain);

	// Init RTVs and DSV heap related resources
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.NumDescriptors = m_RTVBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	context->CreateRTVHeapDescriptor(rtvHeapDesc, &m_pRtvHeap);
	m_RTVDescriptorSize = context->GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	rtvHeapDesc.NumDescriptors = 1;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	context->CreateDSVHeapDescriptor(dsvHeapDesc, &m_pDsvHeap);
	m_DSVDescriptorSize = context->GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void SwapChain::Destroy()
{
	RELEASE_PTR(m_pSwapChain);
	for (int i = 0; i < m_RTVBufferCount; ++i)
	{
		RELEASE_PTR(m_pSwapChainBuffers[i]);
	}
	
	RELEASE_PTR(m_pDepthStencilBuffer);
	RELEASE_PTR(m_pRtvHeap);
	RELEASE_PTR(m_pDsvHeap);
}

}