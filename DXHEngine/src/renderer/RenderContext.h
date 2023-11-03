#pragma once
#include "vendor/d3dx12.h"

struct DXGI_SWAP_CHAIN_DESC;
struct IDXGISwapChain;
struct IDXGIFactory4;

namespace DXH
{
/// <summary>
/// The RenderContext class is a wrapper for the D3D12 device and factory.
/// </summary>
class RenderContext
{
public:
    RenderContext();
    ~RenderContext();

    /// <summary>
    /// Creates the device and factory.
    /// </summary>
    void Init();

    /// <summary>
    /// Destroys the device and factory.
    /// </summary>
    void Destroy();

    /// <summary>
    /// Creates the command queue, allocator and list.
    /// </summary>
    /// <param name="commandQueue">Pointer to the command queue</param>
    /// <param name="commandAllocator">Pointer to the command allocator</param>
    /// <param name="commandList">Pointer to the command list</param>
    void CreateCommandObjects(ID3D12CommandQueue** commandQueue, ID3D12CommandAllocator** commandAllocator, ID3D12GraphicsCommandList** commandList);

    /// <summary>
    /// Creates a fence.
    /// </summary>
    /// <param name="fence">Pointer to the fence</param>
    void CreateFence(ID3D12Fence** fence);

    /// <summary>
    /// Creates a swap chain.
    /// </summary>
    /// <param name="commandQueue">Pointer to the command queue</param>
    /// <param name="swapChainDesc">Description of the swap chain</param>
    /// <param name="swapChain">Pointer to the swap chain</param>
    void CreateSwapChain(ID3D12CommandQueue* commandQueue, DXGI_SWAP_CHAIN_DESC& swapChainDesc, IDXGISwapChain** swapChain);

    /// <summary>
    /// Creates a render target descriptor heap.
    /// </summary>
    /// <param name="numDescriptors">Number of descriptors in the heap</param>
    /// <param name="rtvHeap">Pointer to the heap</param>
    void CreateRTVHeapDescriptor(uint32_t numDescriptors, ID3D12DescriptorHeap** rtvHeap);

    /// <summary>
    /// Creates a depth stencil descriptor heap.
    /// </summary>
    /// <param name="numDescriptors">Number of descriptors in the heap</param>
    /// <param name="dsvHeap">Pointer to the heap</param>
    void CreateDSVHeapDescriptor(uint32_t numDescriptors, ID3D12DescriptorHeap** dsvHeap);

    /// <summary>
    /// Creates a constant buffer view, shader resource view and unordered access view heap.
    /// </summary>
    /// <param name="numDescriptors">Number of descriptors in the heap</param>
    /// <param name="cbvSrvUavHeap">Pointer to the heap</param>
    void CreateCBVSRVUAVHeapDescriptor(uint32_t numDescriptors, ID3D12DescriptorHeap** cbvSrvUavHeap);

    /// <summary>
    /// Creates the render target descriptor
    /// </summary>
    /// <param name="rtv">Pointer to the render target resource</param>
    /// <param name="rtvHeapHandle">Handle to the render target descriptor heap</param>
    void CreateRTV(ID3D12Resource* rtv, CD3DX12_CPU_DESCRIPTOR_HANDLE& rtvHeapHandle)
    {
        m_pDevice->CreateRenderTargetView(rtv, nullptr, rtvHeapHandle);
    }

    /// <summary>
    /// Creates the depth stencil descriptor
    /// </summary>
    /// <param name="dsv">Pointer to the depth stencil resource</param>
    /// <param name="dsvHeapHandle">Handle to the depth stencil descriptor heap</param>
    /// <param name="heapProps">Heap properties</param>
    /// <param name="depthStencilDesc">Description of the depth stencil resource</param>
    /// <param name="optClear">Clear value</param>
    /// <param name="dsvDesc">Description of the depth stencil view</param>
    void CreateDSV(
        ID3D12Resource** dsv, 
        D3D12_CPU_DESCRIPTOR_HANDLE& dsvHeapHandle,
        CD3DX12_HEAP_PROPERTIES& heapProps,
        D3D12_RESOURCE_DESC& depthStencilDesc, 
        D3D12_CLEAR_VALUE& optClear,
        D3D12_DEPTH_STENCIL_VIEW_DESC& dsvDesc);

    /// <summary>
    /// Gets the descriptor size of one element in the descriptor heap.
    /// </summary>
    /// <param name="descriptorHeadType">Type of the descriptor heap</param>
    uint32_t GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeadType)
    { 
        return m_pDevice->GetDescriptorHandleIncrementSize(descriptorHeadType);
    }

    /// <summary>
    /// Creates a committed resource.
    /// </summary>
    /// <param name="res">Pointer to the resource</param>
    /// <param name="heapProps">Heap properties</param>
    /// <param name="bufferDesc">Description of the resource</param>
    /// <param name="state">State of the resource</param>
    void CreateResource(ID3D12Resource** res, const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_RESOURCE_DESC& bufferDesc, D3D12_RESOURCE_STATES state);

    /// <summary>
    /// Creates a root signature.
    /// </summary>
    /// <param name="serializedRootSignature">Pointer to the serialized root signature</param>
    /// <param name="rs">Pointer to the root signature</param>
    void CreateRootSignature(ID3DBlob* serializedRootSignature, ID3D12RootSignature** rs);

    /// <summary>
    /// Creates a graphics pipeline state object.
    /// </summary>
    /// <param name="psoDesc">Description of the PSO</param>
    /// <param name="pso">Pointer to the PSO</param>
    void CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& psoDesc, ID3D12PipelineState** pso);

    /// <summary>
    /// Creates a constant buffer view.
    /// </summary>
    /// <param name="bufferLocation">GPU virtual address of the buffer</param>
    /// <param name="sizeInBytes">Size of the buffer</param>
    /// <param name="cbvSrvHeap">Pointer to the descriptor heap</param>
    /// <param name="heapIndex">Index of the descriptor in the heap</param>
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