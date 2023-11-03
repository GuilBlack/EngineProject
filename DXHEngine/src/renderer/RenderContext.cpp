#include "RenderContext.h"

namespace DXH
{
RenderContext::RenderContext()
{
    Init();
}

RenderContext::~RenderContext()
{
    Destroy();
}

void RenderContext::Init()
{
    ASSERT_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&m_pDXGIFactory)));

    HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));

    if (FAILED(hr))
    {
        IDXGIAdapter* warpAdapter;
        ASSERT_HRESULT(m_pDXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
        ASSERT_HRESULT(D3D12CreateDevice(warpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice)));
    }
}

void RenderContext::Destroy()
{
    RELEASE_PTR(m_pDevice);
    RELEASE_PTR(m_pDXGIFactory);
}

void RenderContext::CreateCommandObjects(ID3D12CommandQueue** commandQueue, ID3D12CommandAllocator** commandAllocator, ID3D12GraphicsCommandList** commandList)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc =
    {
        .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
        .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
    };

    ASSERT_HRESULT(m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(commandQueue)));
    ASSERT_HRESULT(m_pDevice->CreateCommandAllocator(queueDesc.Type, IID_PPV_ARGS(commandAllocator)));
    ASSERT_HRESULT(m_pDevice->CreateCommandList(
        queueDesc.NodeMask,
        queueDesc.Type,
        *commandAllocator, nullptr,
        IID_PPV_ARGS(commandList)));

    (*commandList)->Close();
}

inline void RenderContext::CreateFence(ID3D12Fence** fence)
{
    ASSERT_HRESULT(m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence)));
}

void RenderContext::CreateSwapChain(ID3D12CommandQueue* commandQueue, DXGI_SWAP_CHAIN_DESC& swapChainDesc, IDXGISwapChain** swapChain)
{
    ASSERT_HRESULT(m_pDXGIFactory->CreateSwapChain(commandQueue, &swapChainDesc, swapChain));
}

void RenderContext::CreateRTVHeapDescriptor(uint32_t numDescriptors, ID3D12DescriptorHeap** rtvHeap)
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc =
    {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = numDescriptors,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
    };
    ASSERT_HRESULT(m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(rtvHeap)));
}

inline void RenderContext::CreateDSVHeapDescriptor(uint32_t numDescriptors, ID3D12DescriptorHeap** dsvHeap)
{
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc =
    {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
        .NumDescriptors = numDescriptors,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
    };
    ASSERT_HRESULT(m_pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap)));
}

inline void RenderContext::CreateCBVSRVUAVHeapDescriptor(uint32_t numDescriptors, ID3D12DescriptorHeap** cbvSrvUavHeap)
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc =
    {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        .NumDescriptors = numDescriptors,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
    };
    ASSERT_HRESULT(m_pDevice->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(cbvSrvUavHeap)));
}

void RenderContext::CreateDSV(
    ID3D12Resource** dsv,
    D3D12_CPU_DESCRIPTOR_HANDLE& dsvHeapHandle,
    CD3DX12_HEAP_PROPERTIES& heapProps,
    D3D12_RESOURCE_DESC& depthStencilDesc,
    D3D12_CLEAR_VALUE& optClear,
    D3D12_DEPTH_STENCIL_VIEW_DESC& dsvDesc)
{
    ASSERT_HRESULT(m_pDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(dsv)
    ));

    m_pDevice->CreateDepthStencilView(*dsv, &dsvDesc, dsvHeapHandle);
}

inline void RenderContext::CreateResource(ID3D12Resource** res, const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_RESOURCE_DESC& bufferDesc, D3D12_RESOURCE_STATES state)
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

inline void RenderContext::CreateRootSignature(ID3DBlob* serializedRootSignature, ID3D12RootSignature** rs)
{
    ASSERT_HRESULT(m_pDevice->CreateRootSignature(
        0,
        serializedRootSignature->GetBufferPointer(),
        serializedRootSignature->GetBufferSize(),
        IID_PPV_ARGS(rs)
    ));
}

inline void RenderContext::CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& psoDesc, ID3D12PipelineState** pso)
{
    ASSERT_HRESULT(m_pDevice->CreateGraphicsPipelineState(
        &psoDesc,
        IID_PPV_ARGS(pso)
    ));
}
}
