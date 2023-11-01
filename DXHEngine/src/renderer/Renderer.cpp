#include "Renderer.h"
#include "Geometry.h"
#include "RendererResource.h"
#include "UploadBuffer.h"
#include "Shader.h"
#include "Material.h"
#include "src/ecs/components/Transform.h"
#include "src/ecs/components/Render.h"
#include "src/ecs/components/Camera.h"
#include "src/core/Window.h"

namespace DXH
{
void Renderer::Init()
{
	using namespace DirectX;
	// Init device and stuff
	m_pRenderContext = new RenderContext();

	m_pRenderContext->CreateFence(&m_pFence);

	// Init command related objects
	m_pRenderContext->CreateCommandObjects(&m_pCommandQueue, &m_pCommandAllocator, &m_pCommandList);

	// Create swap chain
	m_pSwapChain = new SwapChain();

	m_pSwapChain->Init(m_pRenderContext, m_pCommandQueue);

	// Create descriptor heap
	m_pRenderContext->CreateCBVSRVUAVHeapDescriptor(10, &m_pCbvSrvHeap);

	RendererResource::GetInstance().Init();
	BaseShader::s_ObjectCB = std::vector<UploadBuffer<ObjectConstants>>();
	BaseShader::s_ObjectCB.reserve(MAX_GO_COUNT);

	//XMVECTOR pos = XMVectorSet(0.f, 0.f, -10.f, 1.f);
	//XMVECTOR target = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	//XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 1.f);
	//XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	//XMStoreFloat4x4(&m_Camera.View, view);
}

void Renderer::Destroy()
{
	for (auto cb : BaseShader::s_ObjectCB)
	{
		cb.Destroy();
	}
	for (auto [_, shader] : RendererResource::GetInstance().m_Shaders)
	{
		shader->m_PassCB.Destroy();
	}
	RELEASE_PTR(m_pFence);
	RELEASE_PTR(m_pCbvSrvHeap);
	RELEASE_PTR(m_pCommandQueue);
	RELEASE_PTR(m_pCommandList);
	RELEASE_PTR(m_pCommandAllocator);
	delete m_pSwapChain;
	delete m_pRenderContext;
}

void Renderer::BeginFrame(const Camera& camera)
{
	using namespace DirectX;
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

	//ID3D12DescriptorHeap* descriptorHeaps[] = { m_pCbvSrvHeap };
	//m_pCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	Matrix view = camera.View.GetMatrixTransposed();
	Matrix proj = camera.Proj.GetMatrixTransposed();
	Matrix viewProj = camera.GetViewProjectionMatrix().GetMatrixTransposed();

	PassConstants passCB =
	{
		.View = view,
		.Proj = proj,
		.ViewProj = viewProj,
		.EyePosW = {0.f, 0.f, -5.f},
		.NearZ = camera.NearPlan,
		.FarZ = camera.FarPlan,
		.TotalTime = 0.f,
		.RenderTargetSize = 
		{
			(float)Window::GetInstance().GetWidth(), 
			(float)Window::GetInstance().GetHeight()
		},
		.DeltaTime = 0.f,
	};

	for (auto [_, shader] : RendererResource::GetInstance().m_Shaders)
	{
		shader->UpdatePassCB(passCB);
	}
}

void Renderer::Draw(Mesh& mesh, Transform& transform)
{
	mesh.Mat->Shader->Bind(m_pCommandList);
	switch (mesh.Mat->Shader->GetType())
	{
	case ShaderProgramType::SimpleShader:
	{
		mesh.Mat->Shader->Draw(mesh.Geo, mesh.GetCBIndex(), transform, m_pCommandList);
		break;
	}
	default:
		break;
	}
	mesh.Mat->Shader->Unbind(m_pCommandList);
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
	using namespace DirectX;
	assert(m_pRenderContext);
	assert(m_pSwapChain);
	assert(m_pCommandAllocator);

	//XMMATRIX proj = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(65.f),
	//	(float)Window::GetInstance().GetWidth() / Window::GetInstance().GetHeight(),
	//	0.001f,
	//	1000.f
	//);
	//XMStoreFloat4x4(&m_Camera.Proj, proj);

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

ID3D12Resource* Renderer::CreateDefaultBuffer(void* data, int64_t byteSize)
{
	FlushCommandQueue();
	ASSERT_HRESULT(m_pCommandList->Reset(m_pCommandAllocator, nullptr));
	ID3D12Resource* uploadBuffer = nullptr;
	ID3D12Resource* defaultBuffer = nullptr;

	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

	m_pRenderContext->CreateResource(&defaultBuffer, heapProperties, resourceDesc, D3D12_RESOURCE_STATE_COMMON);

	heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	m_pRenderContext->CreateResource(&uploadBuffer, heapProperties, resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ);

	D3D12_SUBRESOURCE_DATA subresourceData = 
	{
		.pData = data,
		.RowPitch = byteSize,
		.SlicePitch = byteSize,
	};

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		defaultBuffer,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_COPY_DEST);

	m_pCommandList->ResourceBarrier(1, &barrier);

	UpdateSubresources<1>(m_pCommandList, defaultBuffer, uploadBuffer, 0, 0, 1, &subresourceData);

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		defaultBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ
	);

	m_pCommandList->ResourceBarrier(1, &barrier);
	
	ASSERT_HRESULT(m_pCommandList->Close());
	ID3D12CommandList* commandLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	FlushCommandQueue();
	RELEASE_PTR(uploadBuffer);

	return defaultBuffer;
}
}

