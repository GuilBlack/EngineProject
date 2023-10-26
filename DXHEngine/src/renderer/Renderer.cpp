#include "Renderer.h"

namespace DXH
{
void Renderer::Init(Window* window)
{
	// Init device and stuff
	m_pRenderContext = new RenderContext();

	m_pRenderContext->CreateFence(&m_pFence);

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
	RELEASE_PTR(m_pCommandList);
	RELEASE_PTR(m_pCommandAllocator);
	RELEASE_PTR(m_pCommandQueue);
	delete m_pRenderContext;
}

void Renderer::BeginFrame()
{
	ASSERT_HRESULT(m_pCommandAllocator->Reset());
	ASSERT_HRESULT(m_pCommandList->Reset(m_pCommandAllocator, nullptr));

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pSwapChain->GetCurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_pCommandList->ResourceBarrier(1, &barrier);

	D3D12_VIEWPORT viewport = m_pSwapChain->GetScreenViewport();
	D3D12_RECT scissorRect = m_pSwapChain->GetScissorRect();
	m_pCommandList->RSSetViewports(1, &viewport);
	m_pCommandList->RSSetScissorRects(1, &scissorRect);

	Clear();

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pSwapChain->GetCurrentBackBufferDescriptorHeap();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_pSwapChain->GetDepthStencilDescriptorHeap();

	m_pCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
}

void Renderer::EndFrame()
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pSwapChain->GetCurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	m_pCommandList->ResourceBarrier(1, &barrier);

	ASSERT_HRESULT(m_pCommandList->Close());

	ID3D12CommandList* commandLists[] = {m_pCommandList};
	m_pCommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	m_pSwapChain->Present();

	FlushCommandQueue();
}

void Renderer::OnResize()
{
	assert(m_pRenderContext);
	assert(m_pSwapChain);
	assert(m_pCommandAllocator);

	// Flush before changing any resources.
	FlushCommandQueue();

	ASSERT_HRESULT(m_pCommandList->Reset(m_pCommandAllocator, nullptr));

	m_pSwapChain->ResizeBuffers(m_pRenderContext, m_pCommandList);

	ASSERT_HRESULT(m_pCommandList->Close());
	ID3D12CommandList* commandLists[] = {m_pCommandList};
	m_pCommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	FlushCommandQueue();
}

void Renderer::FlushCommandQueue()
{
	//Increment m_FenceValue
	++m_FenceValue;

	//Signal the m_pCommandQueue to execute all commands using the latest fence value
	ASSERT_HRESULT(m_pCommandQueue->Signal(m_pFence, m_FenceValue));

	//Check if the GPU has completed all commands associated with the previous fence value
	if (m_pFence->GetCompletedValue() < m_FenceValue)
	{
		//Create a new Windows event
		HANDLE eventHandle = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);

		//Set the event to the current fence value
		ASSERT_HRESULT(m_pFence->SetEventOnCompletion(m_FenceValue, eventHandle));

		//Wait for the GPU to complete associated commands
		WaitForSingleObject(eventHandle, INFINITE);

		//Close the handle to the event
		CloseHandle(eventHandle);
	}
}
}

