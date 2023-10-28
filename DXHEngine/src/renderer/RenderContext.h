#pragma once
#include "pch.h"

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

	void CreateCBVSRVUAVHeapDescriptor(uint32_t numDescriptors, ID3D12DescriptorHeap** cbvSrvUavHeap)
	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc =
		{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			.NumDescriptors = numDescriptors,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		};
		ASSERT_HRESULT(m_pDevice->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(cbvSrvUavHeap)));
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

	void CreateResource(ID3D12Resource** res, const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_RESOURCE_DESC& bufferDesc, D3D12_RESOURCE_STATES state)
	{
		ASSERT_HRESULT(m_pDevice->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			state,
			nullptr,
			IID_PPV_ARGS(res)
		));
	}

	void CreateRootSignature(ID3DBlob* serializedRootSignature, ID3D12RootSignature** rs)
	{
			ASSERT_HRESULT(m_pDevice->CreateRootSignature(
					0,
				serializedRootSignature->GetBufferPointer(),
				serializedRootSignature->GetBufferSize(),
				IID_PPV_ARGS(rs)
			));
	}

	void CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& psoDesc, ID3D12PipelineState** pso)
	{
		ASSERT_HRESULT(m_pDevice->CreateGraphicsPipelineState(
			&psoDesc,
			IID_PPV_ARGS(pso)
		));
	}

	void CreateConstantBufferView(D3D12_GPU_VIRTUAL_ADDRESS bufferLocation, uint32_t sizeInBytes, ID3D12DescriptorHeap* cbvSrvHeap, uint32_t heapIndex)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc =
		{
			.BufferLocation = bufferLocation,
			.SizeInBytes = sizeInBytes,
		};

		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			cbvSrvHeap->GetCPUDescriptorHandleForHeapStart(),
			heapIndex,
			m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

		m_pDevice->CreateConstantBufferView(&cbvDesc, handle);
	}

private:
	ID3D12Device* m_pDevice = nullptr;
	IDXGIFactory4* m_pDXGIFactory = nullptr;
};
}