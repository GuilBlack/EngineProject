#pragma once
#include "SwapChain.h"

namespace DXH
{
class Renderer
{
public:
	Renderer() {}
	~Renderer() {}

	void Init();
	void Destroy();

	static Renderer& GetInstance()
	{
		static Renderer instance;
		return instance;
	}

private:
	RenderContext* m_pRenderContext = nullptr;
	SwapChain* m_pSwapChain = nullptr;
	ID3D12CommandQueue* m_pCommandQueue = nullptr;
	ID3D12CommandAllocator* m_pCommandAllocator = nullptr;
	ID3D12GraphicsCommandList* m_pCommandList = nullptr;
};
}


