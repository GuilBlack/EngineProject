#include "SwapChain.h"
#include "src/core/Window.h"

namespace DXH
{

void SwapChain::Init(RenderContext* context, ID3D12CommandQueue* commandQueue)
{
    // Init directx12 swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc =
    {
        .BufferDesc =
        {
            .Width = (uint32_t)Window::GetInstance().GetWidth(),
            .Height = (uint32_t)Window::GetInstance().GetHeight(),
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
        .OutputWindow = Window::GetInstance().GetWindowHandle(),
        .Windowed = true,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    };

    context->CreateSwapChain(commandQueue, swapChainDesc, &m_pSwapChain);

    // Init RTVs and DSV heap related resources
    context->CreateRTVHeapDescriptor(m_RTVBufferCount, &m_pRtvHeap);
    m_RTVDescriptorSize = context->GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    context->CreateDSVHeapDescriptor(1, &m_pDsvHeap);
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

void SwapChain::Present()
{
    ASSERT_HRESULT(m_pSwapChain->Present(0, 0));
    m_CurrentBackBuffer = (m_CurrentBackBuffer + 1) % m_RTVBufferCount;
}

void SwapChain::ResizeBuffers(RenderContext* context, ID3D12GraphicsCommandList* commandList)
{
    // Release old resources
    for (int i = 0; i < m_RTVBufferCount; ++i)
    {
        RELEASE_PTR(m_pSwapChainBuffers[i]);
    }
    RELEASE_PTR(m_pDepthStencilBuffer);

    uint32_t width = Window::GetInstance().GetWidth();
    uint32_t height = Window::GetInstance().GetHeight();

    // Resize swap chain
    ASSERT_HRESULT(m_pSwapChain->ResizeBuffers(m_RTVBufferCount, width, height, m_BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

    m_CurrentBackBuffer = 0;

    // Create RTV for each back buffer
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (int i = 0; i < m_RTVBufferCount; ++i)
    {
        ASSERT_HRESULT(m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pSwapChainBuffers[i])));
        context->CreateRTV(m_pSwapChainBuffers[i], rtvHeapHandle);
        rtvHeapHandle.Offset(1, m_RTVDescriptorSize);
    }

    // Create depth stencil buffer
    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear{};
    optClear.Format = m_DepthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;

    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);

    D3D12_CPU_DESCRIPTOR_HANDLE dsvHeapHandle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = m_DepthStencilFormat;
    dsvDesc.Texture2D.MipSlice = 0;

    context->CreateDSV(
        &m_pDepthStencilBuffer,
        dsvHeapHandle,
        heapProps,
        depthStencilDesc,
        optClear,
        dsvDesc);

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_pDepthStencilBuffer,
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_DEPTH_WRITE
    );

    commandList->ResourceBarrier(1, &barrier);

    m_ScreenViewport = 
    {
        .TopLeftX = 0,
        .TopLeftY = 0,
        .Width = static_cast<float>(width),
        .Height = static_cast<float>(height),
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f
    };

    m_ScissorRect = 
    {
        .left = 0,
        .top = 0,
        .right = static_cast<LONG>(width),
        .bottom = static_cast<LONG>(height)
    };
}

}
