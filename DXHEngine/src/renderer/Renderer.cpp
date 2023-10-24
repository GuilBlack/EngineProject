#include "Renderer.h"

namespace DXH
{
void Renderer::Init(Window* window)
{
	// Init device and stuff
	m_pRenderContext = new RenderContext();

	// Init command related objects
	m_pRenderContext->CreateCommandObjects(&m_pCommandQueue, &m_pCommandAllocator, &m_pCommandList);

	// Create swap chain
	m_pSwapChain = new SwapChain();
	m_pSwapChain->Init(window, m_pRenderContext, m_pCommandQueue);
}

void Renderer::Destroy()
{
	delete m_pSwapChain;
	RELEASE_PTR(m_pCommandQueue);
	delete m_pRenderContext;
}
}

