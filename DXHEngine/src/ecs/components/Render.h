#pragma once
#include "../../renderer/Shader.h"
#include "../Component.h" // Base class
#include "src/renderer/RendererResource.h"

namespace DXH
{
struct Geometry;
struct NumberGeometry;
struct Material;

struct Mesh : public Component
{
    Geometry* Geo = nullptr;
    Material* Mat = nullptr;

    virtual void OnDetach() override
    {
        Geo = nullptr;
        Mat = nullptr;
    }

    virtual void OnAssign() override
    {
        if (!isInitialized)
        {
            isInitialized = true;
            CBVIndex = BaseShader::AddObjectCB();
        }
    }

    void SetGeoAndMatByName(const std::string& geometry, const std::string& material)
    {
        Geo = RendererResource::GetGeometry(geometry);
        Mat = RendererResource::GetMaterial(material);
    }

    uint32_t GetCBIndex() const
    {
        return CBVIndex;
    }

private:
    bool isInitialized = false;
    uint32_t CBVIndex = -1;
};

struct NumberUI : public Component
{
    std::string Number = "";
    uint32_t NumCharacters = 0;
    NumberGeometry* Geo = nullptr;

    NumberUI() = default;

    virtual void OnDetach() override;

    virtual void OnAssign() override
    {
        if (!isInitialized)
        {
            isInitialized = true;
            CBVIndex = BaseShader::AddObjectCB();
        }
    }

    void SetNumber(std::string& number)
    {
        Number = number;
    }

    void InitGeometry(uint32_t numCharacters);

    uint32_t GetCBIndex() const
    {
        return CBVIndex;
    }

private:
    bool isInitialized = false;
    uint32_t CBVIndex = -1;
};

}
