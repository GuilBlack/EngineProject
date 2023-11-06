#include "RendererResource.h"
#include "Geometry.h"
#include "Shader.h"
#include "../ecs/components/Render.h"
#include "Material.h"
#include "Texture.h"

namespace DXH
{
RendererResource::~RendererResource()
{
    for (auto[_, shader] : m_Shaders)
        DELETE_PTR(shader);
    for (auto[_, geometry] : m_Geometries)
        DELETE_PTR(geometry);
    for (auto[_, material] : m_Materials)
        DELETE_PTR(material);
    for (auto [_, texture] : m_Textures)
        DELETE_PTR(texture);
}

void RendererResource::Init()
{
    PrivateCreateShader("SimpleShader", "../DXHEngine/res/shaders/color-vs.cso", "../DXHEngine/res/shaders/color-ps.cso", ShaderProgramType::SimpleShader, InputLayoutType::PositionColor);

    PrivateCreateMaterial("SimpleMaterial", MaterialType::Simple, "SimpleShader");
    CreateSphere();
}

void RendererResource::PrivateCreateShader(const std::string& name, const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout)
{
    if (m_Shaders.contains(name))
        return;
    m_Shaders[name] = BaseShader::Create(vsFilePath, psFilePath, type, layout);
}

void RendererResource::PrivateCreateMaterial(const std::string& materialName, MaterialType materialType, const std::string& shaderName)
{
    if (m_Materials.contains(materialName))
        return;

    if (!m_Shaders.contains(shaderName))
        // TODO: Add error handling
        assert(false && "Shader not found");

    switch (materialType)
    {
    case MaterialType::Simple:
    {
        if (m_Shaders[shaderName]->GetType() != ShaderProgramType::SimpleShader) 
        { // TODO: Add error handling
            assert(false && "Shader type mismatch");
            return;
        }
        Material* pMaterial = new Material();
        pMaterial->Shader = m_Shaders[shaderName];
        pMaterial->Type = MaterialType::Simple;
        m_Materials[materialName] = pMaterial;
        break;
    }
    case MaterialType::Lighting:
    {
        BaseShader* pShader = m_Shaders[shaderName];
        if (pShader->GetType() != ShaderProgramType::BasicLightingShader)
        { // TODO: Add error handling
            assert(false && "Shader type mismatch");
            return;
        }
        Material* pMaterial = new SimpleLightingMaterial();
        pMaterial->Shader = pShader;
        pMaterial->Type = MaterialType::Lighting;
        pMaterial->MaterialCBIndex = pShader->AddMaterialCB();
        m_Materials[materialName] = pMaterial;
        break;
    }
    case MaterialType::TextureLighting:
    {
        BaseShader* pShader = m_Shaders[shaderName];
        if (pShader->GetType() != ShaderProgramType::TextureLightingShader)
        { // TODO: Add error handling
            assert(false && "Shader type mismatch");
            return;
        }
        Material* pMaterial = new TextureLightingMaterial();
        pMaterial->Shader = pShader;
        pMaterial->Type = MaterialType::TextureLighting;
        pMaterial->MaterialCBIndex = pShader->AddMaterialCB();
        m_Materials[materialName] = pMaterial;
        break;
    }
    default:
        // TODO: Add error handling
        assert(false && "Material type not supported");
        break;
    }
}

void RendererResource::PrivateCreateTexture(const std::string& textureName, const std::wstring& texturePath)
{
    if (m_Textures.contains(textureName))
        return;

    Texture* pTexture = Renderer::GetInstance().CreateTexture2D(texturePath);

    m_Textures[textureName] = pTexture;
}

void RendererResource::CreateCube()
{
    using namespace DirectX;

    if (m_Geometries.contains("Cube"))
        return;
    std::vector<BasicVertex> vertices =
    {
        BasicVertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
        BasicVertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
        BasicVertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Yellow) }),
        BasicVertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
        BasicVertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
        BasicVertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }),
        BasicVertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::White) }),
        BasicVertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Cyan) })
    };
    vertices.shrink_to_fit();


    std::vector<std::uint16_t> indices =
    {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    indices.shrink_to_fit();

    uint32_t vbByteSize = (uint32_t)vertices.size() * sizeof(BasicVertex);
    uint32_t vertexByteStride = sizeof(BasicVertex);

    m_Geometries["Cube"] = new Geometry(vertices.data(), indices, vbByteSize, vertexByteStride);
    m_Geometries["Cube"]->BoundingSphere = Geometry::ComputeBoundingSphere(vertices);
}

void RendererResource::CreateSquare()
{
    using namespace DirectX;

    if (m_Geometries.contains("Square"))
        return;
    std::vector<BasicVertex> vertices =
    {
        BasicVertex({ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Black) }),
        BasicVertex({ XMFLOAT3(-1.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Red) }),
        BasicVertex({ XMFLOAT3(+1.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Yellow) }),
        BasicVertex({ XMFLOAT3(+1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Green) }),
    };
    vertices.shrink_to_fit();

    std::vector<std::uint16_t> indices =
    {
        0, 1, 2,
        0, 2, 3,
    };

    indices.shrink_to_fit();

    uint32_t vbByteSize = (uint32_t)vertices.size() * sizeof(BasicVertex);
    uint32_t vertexByteStride = sizeof(BasicVertex);

    m_Geometries["Square"] = new Geometry(vertices.data(), indices, vbByteSize, vertexByteStride);
    m_Geometries["Square"]->BoundingSphere = Geometry::ComputeBoundingSphere(vertices);
}

void RendererResource::CreateSphere()
{
    using namespace DirectX;
    float radius = 1.f;
    uint32_t latitude = 32;
    uint32_t longitude = 32;

    uint32_t numVertices = (latitude + 1) * (longitude * 2);
    uint32_t numIndices = 2 * 3 * longitude + 2 * 3 * (latitude - 1) * longitude;

    std::vector<PosNormTexcoordVertex> vertices;
    vertices.resize(numVertices);
    std::vector<std::uint16_t> indices;
    indices.resize(numIndices);

    float latitudeStep = XM_PI / latitude;
    float longitudeStep = XM_2PI / longitude;

    uint32_t count = 0;

    // top vertex
    for (uint32_t i = 1; i <= longitude; ++i)
    {
        vertices[count].Position = { 0.f, radius, 0.f };
        vertices[count].Normal = { 0.f, 1.f, 0.f };
        vertices[count].Texcoord = { (float)i / ((float)longitude + 1.0f), 0.f };
        ++count;
    }

    // middle vertices
    for (uint32_t i = 1; i < latitude + 1; ++i)
    {
        float pLat = (float)i * latitudeStep;
        for (uint32_t j = 0; j < longitude + 1; ++j)
        {
            float pLong = (float)j * longitudeStep;

            XMFLOAT3 point =
            {
                sinf(pLat) * cosf(pLong),
                cosf(pLat),
                sinf(pLat) * sinf(pLong)
            };

            vertices[count].Position = XMFLOAT3(
                radius * point.x,
                radius * point.y,
                radius * point.z
            );

            vertices[count].Normal = point;

            vertices[count].Texcoord =
            {
                (float)j / (float)longitude,
                (float)i / ((float)latitude + 1.0f)
            };

            ++count;
        }
    }

    // bottom vertex
    for (uint32_t i = 1; i <= longitude; ++i)
    {
        vertices[count].Position = { 0.f, -radius, 0.f };
        vertices[count].Normal = { 0.f, -1.f, 0.f };
        vertices[count].Texcoord = { (float)i / ((float)longitude + 1.0f), 1.f };
        ++count;
    }

    count = 0;

    for (uint32_t i = 0; i < longitude; ++i)
    {
        indices[count++] = i;
        indices[count++] = (longitude-1) + i + 2;
        indices[count++] = (longitude - 1) + i + 1;
    }

    for (uint32_t i = 0; i < latitude - 1; ++i)
    {
        for (uint32_t j = 0; j < longitude; ++j)
        {
            uint32_t index[4] = {
                longitude + i * (longitude + 1) + j,
                longitude + i * (longitude + 1) + (j + 1),
                longitude + (i + 1) * (longitude + 1) + (j + 1),
                longitude + (i + 1) * (longitude + 1) + j
            };

            indices[count++] = index[0];
            indices[count++] = index[1];
            indices[count++] = index[2];

            indices[count++] = index[0];
            indices[count++] = index[2];
            indices[count++] = index[3];
        }
    }

    const uint32_t southPoleIndex = numVertices - longitude;
    for (uint32_t i = 0; i < longitude; ++i)
    {
        indices[count++] = southPoleIndex + i;
        indices[count++] = southPoleIndex - (longitude + 1) + i;
        indices[count++] = southPoleIndex - (longitude + 1) + i + 1;
    }

    uint32_t vbByteSize = (uint32_t)vertices.size() * sizeof(PosNormTexcoordVertex);
    uint32_t vertexByteStride = sizeof(PosNormTexcoordVertex);

    m_Geometries["Sphere"] = new Geometry(vertices.data(), indices, vbByteSize, vertexByteStride);
    m_Geometries["Sphere"]->BoundingSphere = Geometry::ComputeBoundingSphere(vertices);
}
}
