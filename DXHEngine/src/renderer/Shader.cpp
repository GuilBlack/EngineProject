#include "Geometry.h"
#include "Renderer.h"
#include "src/ecs/GameObject.h"

#include "Shader.h"
#include "Material.h"
#include "Util.h"
#include "Texture.h"

namespace DXH
{
std::vector<UploadBuffer<ObjectConstants>> BaseShader::s_ObjectCB;
//////////////////////////////////////////////////////////////////////////
// BaseShader ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BaseShader::BaseShader()
    : m_PassCB(1, true)
{
    m_PassCB.CopyData(0, PassConstants());
}

BaseShader::~BaseShader()
{
    RELEASE_PTR(m_pVS);
    RELEASE_PTR(m_pPS);
    RELEASE_PTR(m_pPSO);
    RELEASE_PTR(m_pRootSignature);
}

BaseShader* BaseShader::Create(const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout)
{
    BaseShader* shader = nullptr;

    switch (type)
    {
        case ShaderProgramType::SimpleShader:
        {
            shader = new SimpleShader();
            break;
        }
        case ShaderProgramType::BasicLightingShader:
        {
            shader = new BasicLightingShader();
            break;
        }
        case ShaderProgramType::TextureLightingShader:
        {
            shader = new TextureLightingShader();
            break;
        }
        case ShaderProgramType::NumberUIShader:
        {
            shader = new NumberUIShader();
            break;
        }
        case ShaderProgramType::ParticleShader:
        {
            shader = new ParticleShader();
            break;
        }
        case ShaderProgramType::SkyboxShader:
        {
            shader = new SkyboxShader();
            break;
        }
    }
    assert(shader && "Wrong shader program type given!");

    shader->m_Type = type;
    shader->m_pVS = LoadCompiledShader(vsFilePath);
    shader->m_pPS = LoadCompiledShader(psFilePath);
    shader->m_InputLayout = CreateInputLayout(layout);

    shader->BuildPSO();

    return shader;
}

void BaseShader::Draw(Geometry* geometry, uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl)
{
    SetCbvSrv(objectCBIndex, material, gameObject, cl);
    D3D12_VERTEX_BUFFER_VIEW vbv = geometry->VertexBufferView();
    D3D12_INDEX_BUFFER_VIEW ibv = geometry->IndexBufferView();

    cl->IASetVertexBuffers(0, 1, &vbv);
    cl->IASetIndexBuffer(&ibv);
    cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    cl->DrawIndexedInstanced(geometry->IndexBufferByteSize / sizeof(uint16_t), 1, 0, 0, 0);
}

void BaseShader::SetCbvSrv(uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl)
{
    using namespace DirectX;
    ObjectConstants objectCB;
    XMStoreFloat4x4(&objectCB.World, XMMatrixTranspose(gameObject.GetModelMatrix()));
    UpdateObjectCB(objectCB, objectCBIndex);
    cl->SetGraphicsRootConstantBufferView(0, s_ObjectCB[objectCBIndex].GetResource()->GetGPUVirtualAddress());
}

uint32_t BaseShader::AddObjectCB()
{
    ObjectConstants objectCB;
    s_ObjectCB.push_back(UploadBuffer<ObjectConstants>());
    s_ObjectCB.back().Init(1, true);
    s_ObjectCB.back().CopyData(0, objectCB);
    return (uint32_t)s_ObjectCB.size() - 1;
}

void BaseShader::UpdatePassCB(PassConstants& passCB)
{
    m_PassCB.CopyData(0, passCB);
}

uint32_t BaseShader::AddMaterialCB()
{
    return -1;
}

void BaseShader::UpdateObjectCB(ObjectConstants& objectCB, uint32_t index)
{
    s_ObjectCB[index].CopyData(0, objectCB);
}

ID3DBlob* BaseShader::LoadCompiledShader(const std::string& filepath)
{
    std::ifstream fin(filepath, std::ios::binary);

    fin.seekg(0, std::ios_base::end);
    uint32_t size = (int)fin.tellg();
    fin.seekg(0, std::ios_base::beg);

    ID3DBlob* bin;
    ASSERT_HRESULT(D3DCreateBlob(size, &bin));
    fin.read((char*)bin->GetBufferPointer(), size);

    fin.close();

    return bin;
}

std::vector<D3D12_INPUT_ELEMENT_DESC> BaseShader::CreateInputLayout(InputLayoutType layout)
{
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

    switch (layout)
    {
    case InputLayoutType::Position:
        inputLayout =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };
        break;
    case InputLayoutType::PositionColor:
        inputLayout =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };
        break;
    case InputLayoutType::PositionNormal:
        inputLayout =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };
        break;
    case InputLayoutType::PositionNormalTexcoord:
        inputLayout =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };
        break;
    case InputLayoutType::PositionNormalColor:
        inputLayout =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };
        break;
    default:
        assert(false && "this layout isn't implemented yet!");
    }
    return inputLayout;
}

void BaseShader::BuildRootSignature(CD3DX12_ROOT_SIGNATURE_DESC& rootSignatureDesc)
{
    ID3DBlob* serializedRootSignature = nullptr;
    ID3DBlob* error = nullptr;

    HRESULT hr = S_OK;

    hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &error);

    ASSERT_HRESULT(hr);

    Renderer::GetRenderContext()->CreateRootSignature(
        serializedRootSignature,
        &m_pRootSignature
    );

    RELEASE_PTR(serializedRootSignature);
    RELEASE_PTR(error);
}

void BaseShader::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc =
    {
        .pRootSignature = m_pRootSignature,
        .VS = 
        {
            reinterpret_cast<BYTE*>(m_pVS->GetBufferPointer()),
            m_pVS->GetBufferSize()
        },
        .PS = 
        {
            reinterpret_cast<BYTE*>(m_pPS->GetBufferPointer()),
            m_pPS->GetBufferSize()
        },
        .BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT),
        .SampleMask = UINT_MAX,
        .RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
        .DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT),
        .InputLayout = { m_InputLayout.data(), (uint32_t)m_InputLayout.size() },
        .PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        .NumRenderTargets = 1,
        .DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT,
        .SampleDesc = { 1, 0 },
        .NodeMask = 0
    };
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

    if (m_IsTransparent)
    {
        D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc =
        {
            .BlendEnable = true,
            .LogicOpEnable = false,
            .SrcBlend = D3D12_BLEND_SRC_ALPHA,
            .DestBlend = D3D12_BLEND_INV_SRC_ALPHA,
            .BlendOp = D3D12_BLEND_OP_ADD,
            .SrcBlendAlpha = D3D12_BLEND_ONE,
            .DestBlendAlpha = D3D12_BLEND_ZERO,
            .BlendOpAlpha = D3D12_BLEND_OP_ADD,
            .LogicOp = D3D12_LOGIC_OP_NOOP,
            .RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL
        };
        
        psoDesc.BlendState.RenderTarget[0] = transparencyBlendDesc;
    }

    Renderer::GetRenderContext()->CreatePSO(psoDesc, &m_pPSO);
}

//////////////////////////////////////////////////////////////////////////
// SimpleShader //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SimpleShader::SimpleShader()
    : BaseShader()
{
    m_Type = ShaderProgramType::SimpleShader;
    CD3DX12_ROOT_PARAMETER rootParameters[2];

    rootParameters[0].InitAsConstantBufferView(0); // b0
    rootParameters[1].InitAsConstantBufferView(1); // b1

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.Init(
        2, rootParameters,
        0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    BuildRootSignature(rootSignatureDesc);
}

void SimpleShader::Bind(ID3D12GraphicsCommandList* cl)
{
    BaseShader::Bind(cl);
    cl->SetGraphicsRootConstantBufferView(1, m_PassCB.GetResource()->GetGPUVirtualAddress());
}

//////////////////////////////////////////////////////////////////////////
// BasicLightingShader //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BasicLightingShader::BasicLightingShader()
{
    m_Type = ShaderProgramType::BasicLightingShader;
    m_MaterialCB.reserve(64);

    CD3DX12_ROOT_PARAMETER rootParameters[3];

    rootParameters[0].InitAsConstantBufferView(0); // b0 objCB
    rootParameters[1].InitAsConstantBufferView(1); // b1 matCB
    rootParameters[2].InitAsConstantBufferView(2); // b2 passCB

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.Init(
        3, rootParameters,
        0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    BuildRootSignature(rootSignatureDesc);
}

BasicLightingShader::~BasicLightingShader()
{
    for (auto& matCB : m_MaterialCB)
        matCB.Destroy();
}

void BasicLightingShader::Bind(ID3D12GraphicsCommandList* cl)
{
    BaseShader::Bind(cl);
    cl->SetGraphicsRootConstantBufferView(2, m_PassCB.GetResource()->GetGPUVirtualAddress()); // passCB
}

void BasicLightingShader::SetCbvSrv(uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl)
{
    using namespace DirectX;
    ObjectConstants objectCB;
    XMStoreFloat4x4(&objectCB.World, XMMatrixTranspose(gameObject.GetModelMatrix()));
    UpdateObjectCB(objectCB, objectCBIndex);
    cl->SetGraphicsRootConstantBufferView(0, s_ObjectCB[objectCBIndex].GetResource()->GetGPUVirtualAddress()); // objCB

    SimpleLightingMaterial* spMat = dynamic_cast<SimpleLightingMaterial*>(material);

    LightingMaterialConstants materialCB;
    materialCB.DiffuseAlbedo = spMat->DiffuseAlbedo;
    materialCB.FresnelR0 = spMat->FresnelR0;
    materialCB.Roughness = spMat->Roughness;

    m_MaterialCB[spMat->MaterialCBIndex].CopyData(0, materialCB);

    cl->SetGraphicsRootConstantBufferView(1, m_MaterialCB[spMat->MaterialCBIndex].GetResource()->GetGPUVirtualAddress()); // matCB
}

uint32_t BasicLightingShader::AddMaterialCB()
{
    UploadBuffer<LightingMaterialConstants> materialCB;
    m_MaterialCB.emplace_back(materialCB);
    m_MaterialCB.back().Init(1, true);
    m_MaterialCB.back().CopyData(0, LightingMaterialConstants());
    return (uint32_t)m_MaterialCB.size() - 1;
}

//////////////////////////////////////////////////////////////////////////
// TextureLightingShader /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

TextureLightingShader::TextureLightingShader()
{
    m_Type = ShaderProgramType::TextureLightingShader;
    m_MaterialCB.reserve(64);

    CD3DX12_ROOT_PARAMETER rootParameters[4];

    CD3DX12_DESCRIPTOR_RANGE texTable;
    texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    // Diffuse texture t0
    rootParameters[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL); // t0
    rootParameters[1].InitAsConstantBufferView(0); // b0 objCB
    rootParameters[2].InitAsConstantBufferView(1); // b1 matCB
    rootParameters[3].InitAsConstantBufferView(2); // b2 passCB

    auto staticSamplers = Renderer::GetInstance().GetStaticSamplers();

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.Init(
        4, rootParameters,
        (uint32_t)staticSamplers.size(), staticSamplers.data(),
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    BuildRootSignature(rootSignatureDesc);
}

TextureLightingShader::~TextureLightingShader()
{
    for (auto& matCB : m_MaterialCB)
        matCB.Destroy();
}

void TextureLightingShader::Bind(ID3D12GraphicsCommandList * cl)
{
    BaseShader::Bind(cl);
    cl->SetGraphicsRootConstantBufferView(3, m_PassCB.GetResource()->GetGPUVirtualAddress()); // passCB
}

void TextureLightingShader::SetCbvSrv(uint32_t objectCBIndex, Material * material, GameObject& gameObject, ID3D12GraphicsCommandList * cl)
{
    using namespace DirectX;

    ObjectConstants objectCB;
    XMStoreFloat4x4(&objectCB.World, XMMatrixTranspose(gameObject.GetModelMatrix()));
    UpdateObjectCB(objectCB, objectCBIndex);
    cl->SetGraphicsRootConstantBufferView(1, s_ObjectCB[objectCBIndex].GetResource()->GetGPUVirtualAddress()); // objCB

    TextureLightingMaterial* pMat = dynamic_cast<TextureLightingMaterial*>(material);

    CD3DX12_GPU_DESCRIPTOR_HANDLE tex(Renderer::GetInstance().GetSrvHeap()->GetGPUDescriptorHandleForHeapStart());
    tex.Offset(pMat->DiffuseTexture->heapIndex, Renderer::GetRenderContext()->GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

    cl->SetGraphicsRootDescriptorTable(0, tex); // t0

    LightingMaterialConstants materialCB;
    materialCB.DiffuseAlbedo = pMat->DiffuseAlbedo;
    materialCB.FresnelR0 = pMat->FresnelR0;
    materialCB.Roughness = pMat->Roughness;

    m_MaterialCB[pMat->MaterialCBIndex].CopyData(0, materialCB);

    cl->SetGraphicsRootConstantBufferView(2, m_MaterialCB[pMat->MaterialCBIndex].GetResource()->GetGPUVirtualAddress()); // matCB
}

uint32_t TextureLightingShader::AddMaterialCB()
{
    UploadBuffer<LightingMaterialConstants> materialCB;
    m_MaterialCB.emplace_back(materialCB);
    m_MaterialCB.back().Init(1, true);
    m_MaterialCB.back().CopyData(0, LightingMaterialConstants());
    return (uint32_t)m_MaterialCB.size() - 1;
}

//////////////////////////////////////////////////////////////////////////
// NumberUIShader ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

NumberUIShader::NumberUIShader()
{
    m_PassCB.CopyData(0, PassConstants());
    m_Type = ShaderProgramType::NumberUIShader;
    m_IsTransparent = true;

    CD3DX12_ROOT_PARAMETER rootParameters[3];

    CD3DX12_DESCRIPTOR_RANGE texTable;
    texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    // Diffuse texture t0
    rootParameters[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL); // t0
    rootParameters[1].InitAsConstantBufferView(0); // b0 objCB
    rootParameters[2].InitAsConstantBufferView(1); // b1 passCB

    auto staticSamplers = Renderer::GetInstance().GetStaticSamplers();

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.Init(
        3, rootParameters,
        (uint32_t)staticSamplers.size(), staticSamplers.data(),
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    BuildRootSignature(rootSignatureDesc);
}

void NumberUIShader::Bind(ID3D12GraphicsCommandList* cl)
{
    BaseShader::Bind(cl);
    cl->SetGraphicsRootConstantBufferView(2, m_PassCB.GetResource()->GetGPUVirtualAddress()); // passCB
}

void NumberUIShader::SetCbvSrv(uint32_t objectCBIndex, Material * material, GameObject & gameObject, ID3D12GraphicsCommandList * cl)
{
    using namespace DirectX;

    ObjectConstants objectCB;
    XMStoreFloat4x4(&objectCB.World, XMMatrixTranspose(gameObject.GetModelMatrix()));
    UpdateObjectCB(objectCB, objectCBIndex);
    cl->SetGraphicsRootConstantBufferView(1, s_ObjectCB[objectCBIndex].GetResource()->GetGPUVirtualAddress()); // objCB

    NumberUIMaterial* pMat = dynamic_cast<NumberUIMaterial*>(material);

    CD3DX12_GPU_DESCRIPTOR_HANDLE tex(Renderer::GetInstance().GetSrvHeap()->GetGPUDescriptorHandleForHeapStart());
    tex.Offset(pMat->NumberAtlas->heapIndex, Renderer::GetRenderContext()->GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

    cl->SetGraphicsRootDescriptorTable(0, tex); // t0
}

//////////////////////////////////////////////////////////////////////////
// ParticleShader ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ParticleShader::ParticleShader()
{
    m_PassCB.CopyData(0, PassConstants());
    m_Type = ShaderProgramType::ParticleShader;
    m_IsTransparent = true;

    CD3DX12_ROOT_PARAMETER rootParameters[3];

    rootParameters[0].InitAsShaderResourceView(0, 1); // t0
    rootParameters[1].InitAsConstantBufferView(0); // b0
    rootParameters[2].InitAsConstantBufferView(1); // b1

    auto staticSamplers = Renderer::GetInstance().GetStaticSamplers();

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.Init(
        3, rootParameters,
        (uint32_t)staticSamplers.size(), staticSamplers.data(),
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    BuildRootSignature(rootSignatureDesc);
}

void ParticleShader::Bind(ID3D12GraphicsCommandList* cl)
{
    BaseShader::Bind(cl);
    cl->SetGraphicsRootConstantBufferView(2, m_PassCB.GetResource()->GetGPUVirtualAddress()); // passCB
}

void ParticleShader::DrawInstanced(Geometry* geometry, GameObject& gameObject, uint32_t objectCBIndex, UploadBuffer<ParticleConstants>& instanceData, Material* material, ID3D12GraphicsCommandList* cl, uint32_t instanceCount)
{
    using namespace DirectX;

    ObjectConstants objectCB;
    XMStoreFloat4x4(&objectCB.World, XMMatrixTranspose(gameObject.GetModelMatrix()));
    UpdateObjectCB(objectCB, objectCBIndex);
    cl->SetGraphicsRootConstantBufferView(1, s_ObjectCB[objectCBIndex].GetResource()->GetGPUVirtualAddress()); // b0

    D3D12_VERTEX_BUFFER_VIEW vbv = geometry->VertexBufferView();
    D3D12_INDEX_BUFFER_VIEW ibv = geometry->IndexBufferView();

    cl->IASetVertexBuffers(0, 1, &vbv);
    cl->IASetIndexBuffer(&ibv);
    cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    cl->SetGraphicsRootShaderResourceView(0, instanceData.GetResource()->GetGPUVirtualAddress());

    cl->DrawIndexedInstanced(geometry->IndexBufferByteSize / sizeof(uint16_t), instanceCount, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
// SkyboxShader //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SkyboxShader::SkyboxShader()
{
    m_PassCB.CopyData(0, PassConstants());
    m_Type = ShaderProgramType::SkyboxShader;

    CD3DX12_ROOT_PARAMETER rootParameters[3];

    CD3DX12_DESCRIPTOR_RANGE texTable;
    texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    rootParameters[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL); // t0 skybox
    rootParameters[1].InitAsConstantBufferView(0); // b0 objCB
    rootParameters[2].InitAsConstantBufferView(1); // b1 passCB

    auto staticSamplers = Renderer::GetInstance().GetStaticSamplers();

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.Init(
        3, rootParameters,
        (uint32_t)staticSamplers.size(), staticSamplers.data(),
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    BuildRootSignature(rootSignatureDesc);
}

void SkyboxShader::Bind(ID3D12GraphicsCommandList* cl)
{
    BaseShader::Bind(cl);
    cl->SetGraphicsRootConstantBufferView(2, m_PassCB.GetResource()->GetGPUVirtualAddress()); // passCB
}

void SkyboxShader::SetCbvSrv(uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl)
{
    using namespace DirectX;

    ObjectConstants objectCB;
    XMStoreFloat4x4(&objectCB.World, XMMatrixTranspose(gameObject.GetModelMatrix()));
    UpdateObjectCB(objectCB, objectCBIndex);
    cl->SetGraphicsRootConstantBufferView(1, s_ObjectCB[objectCBIndex].GetResource()->GetGPUVirtualAddress()); // objCB

    SkyboxMaterial* pMat = dynamic_cast<SkyboxMaterial*>(material);

    CD3DX12_GPU_DESCRIPTOR_HANDLE tex(Renderer::GetInstance().GetSrvHeap()->GetGPUDescriptorHandleForHeapStart());
    tex.Offset(pMat->SkyboxTexture->heapIndex, Renderer::GetRenderContext()->GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

    cl->SetGraphicsRootDescriptorTable(0, tex); // t0
}

void SkyboxShader::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc =
    {
        .pRootSignature = m_pRootSignature,
        .VS =
        {
            reinterpret_cast<BYTE*>(m_pVS->GetBufferPointer()),
            m_pVS->GetBufferSize()
        },
        .PS =
        {
            reinterpret_cast<BYTE*>(m_pPS->GetBufferPointer()),
            m_pPS->GetBufferSize()
        },
        .BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT),
        .SampleMask = UINT_MAX,
        .RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
        .DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT),
        .InputLayout = { m_InputLayout.data(), (uint32_t)m_InputLayout.size() },
        .PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        .NumRenderTargets = 1,
        .DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT,
        .SampleDesc = { 1, 0 },
        .NodeMask = 0
    };
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

    Renderer::GetRenderContext()->CreatePSO(psoDesc, &m_pPSO);
}
}
