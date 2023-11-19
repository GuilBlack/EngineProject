#pragma once
#include "UploadBuffer.h"
#include "ConstantBuffers.h"
#include <string>

namespace DXH
{
/// <summary>
/// An enumeration for different types of shader programs.
/// </summary>
enum class ShaderProgramType
{
    None,
    SimpleShader,
    BasicLightingShader,
    TextureLightingShader,
    NumberUIShader,
    ParticleShader,
    SkyboxShader
};


/// <summary>
/// An enumeration for different types of input layouts for shaders, representing various vertex formats.
/// </summary>
enum class InputLayoutType
{
    Position,
    PositionColor,
    PositionTexcoord,
    PositionNormal,
    PositionNormalColor,
    PositionNormalTexcoord
};

/// <summary>
/// A struct representing the basic vertex format with position and color information.
/// </summary>
struct BasicVertex
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT4 Color;
};

struct PosNormVertex
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
};

struct PosNormTexcoordVertex
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT2 Texcoord;
};

struct Geometry;
class GameObject;
struct PassConstants;
struct ObjectConstants;
struct ParticleData;

/// <summary>
/// The base class for all shaders, providing common functionality. It includes methods for creating shaders, binding them to a graphics command list, drawing with shaders, unbinding shaders, and updating constant buffers for shaders. It also manages the root signature, input layout, and other shader-related resources.
/// </summary>
class BaseShader
{
    friend class Renderer;
public:
    BaseShader();
    virtual ~BaseShader();

    /// <summary>
    /// Creates a shader program from the given vertex and pixel shader files, with the given shader program type and input layout type.
    /// </summary>
    /// <param name="vsFilePath">The file path to the vertex shader file.</param>
    /// <param name="psFilePath">The file path to the pixel shader file.</param>
    /// <param name="type">The type of shader program to create.</param>
    /// <param name="layout">The type of input layout to use.</param>
    static BaseShader* Create(const std::string& vsFilePath, const std::string& psFilePath, ShaderProgramType type, InputLayoutType layout);

    /// <summary>
    /// Binds the shader to the given graphics command list.
    /// </summary>
    /// <param name="cl">The graphics command list to bind the shader to.</param>
    virtual void Bind(ID3D12GraphicsCommandList* cl) 
    {
        cl->SetPipelineState(m_pPSO);
        cl->SetGraphicsRootSignature(m_pRootSignature);
    }
    
    /// <summary>
    /// Draws the given geometry with the given object constant buffer index, gameObject, and graphics command list.
    /// </summary>
    /// <param name="geometry">The geometry to draw.</param>
    /// <param name="objectCBIndex">The index of the object constant buffer to use.</param>
    /// <param name="gameObject">The transform to use.</param>
    /// <param name="cl">The graphics command list to use.</param>
    virtual void Draw(Geometry* geometry, uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl);

    /// <summary>
    /// Unbinds the shader from the given graphics command list.
    /// </summary>
    virtual void Unbind(ID3D12GraphicsCommandList* cl) 
    {
        cl->SetGraphicsRootSignature(nullptr);
    }

    /// <summary>
    /// Gets the type of the shader program.
    /// </summary>
    /// <returns>The type of the shader program.</returns>
    ShaderProgramType GetType() { return m_Type; }

    /// <summary>
    /// Adds an object constant buffer to the shader and returns its index.
    /// </summary>
    /// <returns>The index of the object constant buffer.</returns>
    static uint32_t AddObjectCB();

    /// <summary>
    /// Updates the pass constant buffer with the given pass constants.
    /// </summary>
    /// <param name="passCB">The pass constants to update the pass constant buffer with.</param>
    void UpdatePassCB(PassConstants& passCB);

    virtual uint32_t AddMaterialCB();

    virtual void UpdateMaterialCB(Material* material) {}

    /// <summary>
    /// Updates the object constant buffer at the given index with the given object constants.
    /// </summary>
    /// <param name="objectCB">The object constants to update the object constant buffer with.</param>
    /// <param name="index">The index of the object constant buffer to update.</param>
    static void UpdateObjectCB(ObjectConstants& objectCB, uint32_t index);

protected:
    ID3DBlob* m_pVS = nullptr;
    ID3DBlob* m_pPS = nullptr;
    
    ID3D12PipelineState* m_pPSO = nullptr;
    ID3D12RootSignature* m_pRootSignature = nullptr;
    std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

    UploadBuffer<PassConstants> m_PassCB;
    ShaderProgramType m_Type = ShaderProgramType::None;

    bool m_IsTransparent = false;

    static std::vector<UploadBuffer<ObjectConstants>> s_ObjectCB;

protected:
    /// <summary>
    /// Loads a compiled shader from the given file path.
    /// </summary>
    /// <param name="filepath">The file path to the compiled shader.</param>
    /// <returns>The compiled shader.</returns>
    static ID3DBlob* LoadCompiledShader(const std::string& filepath);
    static std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(InputLayoutType layout);
    
    /// <summary>
    /// Builds the root signature for the shader.
    /// </summary>
    /// <param name="rootParameters">The root parameters to use.</param>
    void BuildRootSignature(CD3DX12_ROOT_SIGNATURE_DESC& rootSignatureDesc);

    /// <summary>
    /// Builds the pipeline state object for the shader.
    /// </summary>
    virtual void BuildPSO();

    virtual void SetCbvSrv(uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl);
};

//////////////////////////////////////////////////////////////////////////
// SimpleShader //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class SimpleShader : public BaseShader
{
public:
    SimpleShader();

    /// <summary>
    /// Binds the shader to the given graphics command list.
    /// </summary>
    virtual void Bind(ID3D12GraphicsCommandList* cl) override;
private:
};

//////////////////////////////////////////////////////////////////////////
// BasicLightingShader //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BasicLightingShader : public BaseShader
{
public:
    BasicLightingShader();
    ~BasicLightingShader();

    virtual void Bind(ID3D12GraphicsCommandList* cl) override;

    virtual void SetCbvSrv(uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl) override;

    virtual uint32_t AddMaterialCB() override;

private:
    std::vector<UploadBuffer<LightingMaterialConstants>> m_MaterialCB;
};

//////////////////////////////////////////////////////////////////////////
// TextureLightingShader /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class TextureLightingShader : public BaseShader
{
public:
    TextureLightingShader();
    ~TextureLightingShader();

    virtual void Bind(ID3D12GraphicsCommandList* cl) override;

    virtual void SetCbvSrv(uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl) override;

    virtual uint32_t AddMaterialCB() override;

private:
    std::vector<UploadBuffer<LightingMaterialConstants>> m_MaterialCB;
};

//////////////////////////////////////////////////////////////////////////
// NumberUIShader ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class NumberUIShader : public BaseShader
{
public:
    NumberUIShader();
    ~NumberUIShader() {}

    virtual void Bind(ID3D12GraphicsCommandList* cl) override;

    //virtual void Draw(Geometry* geometry, uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl) override;

    virtual void SetCbvSrv(uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl) override;

private:
};

//////////////////////////////////////////////////////////////////////////
// ParticleShader ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class ParticleShader : public BaseShader
{
public:
    ParticleShader();
    ~ParticleShader() {}

    virtual void Bind(ID3D12GraphicsCommandList* cl) override;

    void DrawInstanced(Geometry* geometry, GameObject& gameObject, uint32_t objectCBIndex, UploadBuffer<ParticleConstants>& instanceData, Material* material, ID3D12GraphicsCommandList* cl, uint32_t instanceCount);
};

//////////////////////////////////////////////////////////////////////////
// SkyboxShader //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class SkyboxShader : public BaseShader
{
public:
    SkyboxShader();
    ~SkyboxShader() {}

    virtual void Bind(ID3D12GraphicsCommandList* cl) override;

protected:
    virtual void SetCbvSrv(uint32_t objectCBIndex, Material* material, GameObject& gameObject, ID3D12GraphicsCommandList* cl) override;

    virtual void BuildPSO() override;
};
}
