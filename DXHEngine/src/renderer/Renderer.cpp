#include "Renderer.h"
#include "Geometry.h"
#include "RendererResource.h"
#include "UploadBuffer.h"
#include "Shader.h"
#include "Material.h"
#include "src/ecs/GameObject.h"
#include "src/ecs/components/Render.h"
#include "src/ecs/components/Camera.h"
#include "src/ecs/components/Particles.h"
#include "src/core/Window.h"
#include "src/time/Timer.h"
#include "Texture.h"
#include "vendor/DDSTextureLoader.h"

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
    m_pRenderContext->CreateCBVSRVUAVHeapDescriptor(100, &m_pSrvHeap);

    RendererResource::GetInstance().Init();
    BaseShader::s_ObjectCB = std::vector<UploadBuffer<ObjectConstants>>();
    BaseShader::s_ObjectCB.reserve(MAX_GO_COUNT*2);
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
    RELEASE_PTR(m_pSrvHeap);
    RELEASE_PTR(m_pCommandQueue);
    RELEASE_PTR(m_pCommandList);
    RELEASE_PTR(m_pCommandAllocator);
    delete m_pSwapChain;
    delete m_pRenderContext;
}

void Renderer::BeginFrame(const Camera& camera, const Timer& timer)
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

    ID3D12DescriptorHeap* descriptorHeaps[] = { m_pSrvHeap };
    m_pCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    Matrix orthoProj = XMMatrixOrthographicLH(
        (float)Window::GetInstance().GetWidth(),
        (float)Window::GetInstance().GetHeight(),
        camera.NearPlan,
        camera.FarPlan
    );

    Matrix view = camera.View.GetMatrixTranspose();
    Matrix proj = camera.Proj.GetMatrixTranspose();
    Matrix viewProj = camera.GetViewProjectionMatrix().GetMatrixTranspose();

    PassConstants passCB =
    {
        .View = view,
        .OrthoProj = orthoProj,
        .ViewProj = viewProj,
        .EyePosW = camera.pGameObject->Position(),
        .NearZ = camera.NearPlan,
        .FarZ = camera.FarPlan,
        .TotalTime = timer.TotalTime(),
        .RenderTargetSize = 
        {
            (float)Window::GetInstance().GetWidth(), 
            (float)Window::GetInstance().GetHeight()
        },
        .DeltaTime = timer.DeltaTime(),
        .AmbientIntensity = 0.01f,
        .SunIntensity = .9f,
        .SunDirection = { .0f, -1.f, 1.f },
        .SunColor = { 1.f, 1.f, 1.f }
    };

    for (auto [_, shader] : RendererResource::GetInstance().m_Shaders)
    {
        shader->UpdatePassCB(passCB);
    }
}

void Renderer::Draw(Mesh& mesh, GameObject& gameObject)
{
    mesh.Mat->Shader->Bind(m_pCommandList);
    mesh.Mat->Shader->Draw(mesh.Geo, mesh.GetCBIndex(), mesh.Mat, gameObject, m_pCommandList);
    mesh.Mat->Shader->Unbind(m_pCommandList);
}

void Renderer::DrawNumber(NumberUI& numberUI, GameObject& transform)
{
    if (numberUI.Number.size() != numberUI.NumCharacters)
        return;
    NumberGeometry* geo = numberUI.Geo;

    for (uint32_t i = 0; i < numberUI.NumCharacters; i++)
    {
        float uvStride = .1f;
        float numUV = (numberUI.Number[i] - 48) * uvStride;
        uint32_t uvByteIndex = i * 4 * sizeof(PosNormTexcoordVertex) + sizeof(PosNormVertex);
        geo->VertexBuffer.CopyData(uvByteIndex, Vector2(numUV, 0.f));
        uvByteIndex += sizeof(PosNormTexcoordVertex);
        geo->VertexBuffer.CopyData(uvByteIndex, Vector2(numUV + uvStride, 0.f));
        uvByteIndex += sizeof(PosNormTexcoordVertex);
        geo->VertexBuffer.CopyData(uvByteIndex, Vector2(numUV, 1.f));
        uvByteIndex += sizeof(PosNormTexcoordVertex);
        geo->VertexBuffer.CopyData(uvByteIndex, Vector2(numUV + uvStride, 1.f));
    }
    Material* pMat = RendererResource::GetMaterial("NumberUI");
    pMat->Shader->Bind(m_pCommandList);
    pMat->Shader->Draw(geo, numberUI.GetCBIndex(), pMat, transform, m_pCommandList);
    pMat->Shader->Unbind(m_pCommandList);
}

void Renderer::DrawParticles(Particles particles, GameObject& transform)
{
    Material* pMat = RendererResource::GetMaterial("Particles");
    ParticleShader* shader = static_cast<ParticleShader*>(pMat->Shader);

    shader->Bind(m_pCommandList);
    shader->DrawInstanced(particles.Geo, transform, particles.GetCBIndex(), particles.GetInstanceData(), pMat, m_pCommandList, (uint32_t)particles.ParticlesData.size());
    shader->Unbind(m_pCommandList);
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

void Renderer::Clear()
{
    m_pCommandList->ClearRenderTargetView(
        m_pSwapChain->GetCurrentBackBufferDescriptorHeap(),
        DirectX::Colors::Gray,
        0, nullptr);
    m_pCommandList->ClearDepthStencilView(
        m_pSwapChain->GetDepthStencilDescriptorHeap(),
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
        1.0f, 0, 0, nullptr
    );
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

Texture* Renderer::CreateTexture2D(const std::wstring& texturePath)
{
    using namespace DirectX;

    Microsoft::WRL::ComPtr<ID3D12Resource> uploadHeap = nullptr;

    FlushCommandQueue();
    ASSERT_HRESULT(m_pCommandList->Reset(m_pCommandAllocator, nullptr));

    Texture* pTexture = new Texture();
    ASSERT_HRESULT(CreateDDSTextureFromFile12(
        GetRenderContext()->GetDevice(),
        m_pCommandList,
        texturePath.c_str(),
        pTexture->Resource,
        uploadHeap
    ));
    D3D12_RESOURCE_DESC textureDesc = pTexture->Resource->GetDesc();

    CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_pSrvHeap->GetCPUDescriptorHandleForHeapStart());
    hDescriptor.Offset(m_SrvIndex, m_pRenderContext->GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = 
    {
        .Format = textureDesc.Format,
        .ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D,
        .Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
        .Texture2D = 
        {
            .MostDetailedMip = 0,
            .MipLevels = textureDesc.MipLevels,
            .ResourceMinLODClamp = 0.0f,
        }
    };

    m_pRenderContext->GetDevice()->CreateShaderResourceView(pTexture->Resource.Get(), &srvDesc, hDescriptor);

    ASSERT_HRESULT(m_pCommandList->Close());
    ID3D12CommandList* commandLists[] = { m_pCommandList };
    m_pCommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

    FlushCommandQueue();

    pTexture->heapIndex = m_SrvIndex++;
    return pTexture;
}

Texture* Renderer::CreateTextureCube(const std::wstring& texturePath)
{
    using namespace DirectX;

    Microsoft::WRL::ComPtr<ID3D12Resource> uploadHeap = nullptr;

    FlushCommandQueue();
    ASSERT_HRESULT(m_pCommandList->Reset(m_pCommandAllocator, nullptr));

    Texture* pTexture = new Texture();
    ASSERT_HRESULT(CreateDDSTextureFromFile12(
        GetRenderContext()->GetDevice(),
        m_pCommandList,
        texturePath.c_str(),
        pTexture->Resource,
        uploadHeap
    ));
    D3D12_RESOURCE_DESC textureDesc = pTexture->Resource->GetDesc();

    CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_pSrvHeap->GetCPUDescriptorHandleForHeapStart());
    hDescriptor.Offset(m_SrvIndex, m_pRenderContext->GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc =
    {
        .Format = textureDesc.Format,
        .ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE,
        .Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
        .TextureCube =
        {
            .MostDetailedMip = 0,
            .MipLevels = textureDesc.MipLevels,
            .ResourceMinLODClamp = 0.0f,
        }
    };

    m_pRenderContext->GetDevice()->CreateShaderResourceView(pTexture->Resource.Get(), &srvDesc, hDescriptor);

    ASSERT_HRESULT(m_pCommandList->Close());
    ID3D12CommandList* commandLists[] = { m_pCommandList };
    m_pCommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

    FlushCommandQueue();

    pTexture->heapIndex = m_SrvIndex++;
    return pTexture;
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> Renderer::GetStaticSamplers()
{
    static const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
        0, // shaderRegister
        D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

    static const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
        1, // shaderRegister
        D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

    static const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
        2, // shaderRegister
        D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

    static const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
        3, // shaderRegister
        D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

    static const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
        4, // shaderRegister
        D3D12_FILTER_ANISOTROPIC, // filter
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
        0.0f,                             // mipLODBias
        8);                               // maxAnisotropy

    static const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
        5, // shaderRegister
        D3D12_FILTER_ANISOTROPIC, // filter
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
        0.0f,                              // mipLODBias
        8);                                // maxAnisotropy

    static std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> samplerDescs =
    {
        linearWrap, linearClamp,
        pointWrap, pointClamp,
        anisotropicWrap, anisotropicClamp
    };

    return samplerDescs;
}
}
