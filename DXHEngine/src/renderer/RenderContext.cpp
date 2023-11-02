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

void RenderContext::CreateSwapChain(ID3D12CommandQueue* commandQueue, DXGI_SWAP_CHAIN_DESC& swapChainDesc, IDXGISwapChain** swapChain)
{
    ASSERT_HRESULT(m_pDXGIFactory->CreateSwapChain(commandQueue, &swapChainDesc, swapChain));
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

}

