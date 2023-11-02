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

    /// <summary>
    /// Create a shader
    /// </summary>
    /// <param name="name">Name of the shader</param>
    /// <param name="vsFilePath">Path to the vertex shader</param>
    /// <param name="psFilePath">Path to the pixel shader</param>
    /// <param name="type">Type of the shader program</param>
    /// <param name="layout">Type of the input layout</param>
    void CreateShader(const std::string& name, const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout);

    void CreateMaterial(const std::string& materialName, MaterialType materialType, const std::string& shaderName);

    /// <summary>
    /// Create a Mesh
    /// </summary>
    /// <param name="shaderName">Name of the shader that will be used.</param>
    /// <param name="geometryName">Name of the geometry that will be used.</param>
    Mesh CreateMesh(const std::string& shaderName, const std::string& geometryName);

    /// <summary>
    /// Get a material
    /// </summary>
    /// <param name="materialName">Name of the material</param>
    /// <returns>Pointer to the material</returns>
    static Material* GetMaterial(const std::string& materialName) { return RendererResource::GetInstance().m_Materials[materialName]; }

    static Geometry* GetGeometry(const std::string& geometryName) { return RendererResource::GetInstance().m_Geometries[geometryName]; }

private:
    std::unordered_map<std::string, BaseShader*> m_Shaders;
    std::unordered_map<std::string, Geometry*> m_Geometries;
    std::unordered_map<std::string, Material*> m_Materials;

private:
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