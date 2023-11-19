#pragma once
#include "../maths/Vector3.h"

namespace DXH
{
enum class ShaderProgramType;
enum class InputLayoutType;
class BaseShader;
struct Geometry;
struct Mesh;
struct Material;
enum class MaterialType;
struct Texture;

struct Environment
{
    Vector3 SunDirection = { 0.0f, 0.0f, 0.0f };
    Vector3 SunColor = { 1.0f, 1.0f, 1.0f };
    Vector3 AmbientColor = { 1.0f, 1.0f, 1.0f };
    float AmbientIntensity = 0.1f;
    float SunIntensity = 1.0f;
};

class RendererResource
{
    friend class Renderer;
public:
    RendererResource() = default;
    ~RendererResource();

    /// <summary>
    /// Singleton
    /// </summary>
    static RendererResource& GetInstance()
    {
        static RendererResource instance;
        return instance;
    }

    /// <summary>
    /// Initialize all basic resources
    /// </summary>
    void Init();

    static void CreateShader(const std::string& name, const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout)
    {
        GetInstance().PrivateCreateShader(name, vsFilePath, psFilePath, type, layout);
    }

    static void CreateMaterial(const std::string& materialName, MaterialType materialType, const std::string& shaderName)
    {
        GetInstance().PrivateCreateMaterial(materialName, materialType, shaderName);
    }

    static void CreateTexture(const std::string& textureName, const std::wstring& texturePath)
    {
        GetInstance().PrivateCreateTexture(textureName, texturePath);
    }

    static void CreateTextureCube(const std::string& textureName, const std::wstring& texturePath)
    {
        GetInstance().PrivateCreateTextureCube(textureName, texturePath);
    }

    /// <summary>
    /// Get a material
    /// </summary>
    /// <param name="materialName">Name of the material</param>
    /// <returns>Pointer to the material</returns>
    static Material* GetMaterial(const std::string& materialName) { return RendererResource::GetInstance().m_Materials[materialName]; }

    static Geometry* GetGeometry(const std::string& geometryName) { return RendererResource::GetInstance().m_Geometries[geometryName]; }

    static Texture* GetTexture(const std::string& textureName) { return RendererResource::GetInstance().m_Textures[textureName]; }

    static Texture* GetTextureCube(const std::string& textureName) { return RendererResource::GetInstance().m_TexturesCube[textureName]; }

private:
    std::unordered_map<std::string, BaseShader*> m_Shaders;
    std::unordered_map<std::string, Geometry*> m_Geometries;
    std::unordered_map<std::string, Material*> m_Materials;
    std::unordered_map<std::string, Texture*> m_Textures;
    std::unordered_map<std::string, Texture*> m_TexturesCube;

private:
    /// <summary>
    /// Create a shader
    /// </summary>
    /// <param name="name">Name of the shader</param>
    /// <param name="vsFilePath">Path to the vertex shader</param>
    /// <param name="psFilePath">Path to the pixel shader</param>
    /// <param name="type">Type of the shader program</param>
    /// <param name="layout">Type of the input layout</param>
    void PrivateCreateShader(const std::string& name, const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout);

    void PrivateCreateMaterial(const std::string& materialName, MaterialType materialType, const std::string& shaderName);


    void PrivateCreateTexture(const std::string& textureName, const std::wstring& texturePath);

    void PrivateCreateTextureCube(const std::string& textureName, const std::wstring& texturePath);

    /// <summary>
    /// Create a cube geometry
    /// </summary>
    void CreateCube();

    /// <summary>
    /// Create a square geometry
    /// </summary>
    void CreateSquare();

    /// <summary>
    /// Create a sphere geometry
    /// </summary>
    void CreateSphere();
};
}
