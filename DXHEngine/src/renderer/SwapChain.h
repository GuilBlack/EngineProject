#pragma once
#include "../core/Window.h"
#include "RenderContext.h"

namespace DXH
{
class SwapChain
{
	public:
	SwapChain() = default;
	~SwapChain() { Destroy(); }

	void Init(Window* window, RenderContext* context, ID3D12CommandQueue* commandQueue);
	void Destroy();
	void Present();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferDescriptorHeap()
	{ 
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), m_CurrentBackBuffer, m_RTVDescriptorSize);
	}
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilDescriptorHeap() { return m_pDsvHeap->GetCPUDescriptorHandleForHeapStart(); }
	ID3D12Resource* GetCurrentBackBuffer() { return m_pSwapChainBuffers[m_CurrentBackBuffer]; }
	D3D12_VIEWPORT GetScreenViewport() { return m_ScreenViewport; }
	D3D12_RECT GetScissorRect() { return m_ScissorRect; }

	void ResizeBuffers(RenderContext* context, ID3D12GraphicsCommandList* commandList);

private:
	IDXGISwapChain* m_pSwapChain = nullptr;

	D3D12_VIEWPORT m_ScreenViewport{};
	D3D12_RECT m_ScissorRect{};

	UINT m_RTVDescriptorSize = 0;
	UINT m_DSVDescriptorSize = 0;

	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	bool m_MsaaState = false;
	uint32_t m_MsaaQuality = 0;

	static const uint32_t m_RTVBufferCount = 2;
	uint32_t m_CurrentBackBuffer = 0;
	ID3D12Resource* m_pSwapChainBuffers[m_RTVBufferCount] = { nullptr, nullptr };
	ID3D12Resource* m_pDepthStencilBuffer = nullptr;
	ID3D12DescriptorHeap* m_pRtvHeap = nullptr;
	ID3D12DescriptorHeap* m_pDsvHeap = nullptr;
};
}

