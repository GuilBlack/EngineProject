#pragma once
#include "../Component.h"
#include "DXHMaths.h"

namespace DXH
{
template<typename T>
class UploadBuffer;
struct Geometry;
struct ParticleConstants;

struct ParticleData
{
    Vector3 Pos = Vector3::Zero;
    Vector3 Scale = { 1.f,1.f,1.f };
    Vector3 Velocity = Vector3::Zero;
    float MaxTimeAlive = 4.0f;
    Color BeginColor = Color::White;
    Color EndColor = Color::Black;
    float TimeAlive = 0.0f;
    Vector3 Padding = Vector3(0.f,0.f,0.f);
};

struct Particles : public Component
{
    Geometry* Geo = nullptr;
    std::vector<ParticleData> ParticlesData;

    Particles() {}
    ~Particles() {}

    virtual void OnAssign() override;
    virtual void OnDetach() override;

    uint32_t GetCBIndex() const
    {
        return CBVIndex;
    }

    UploadBuffer<ParticleConstants>& GetInstanceData()
    {
        return *m_ParticlesDataBuffer;
    }

    void UpdateInstanceData(uint32_t index, ParticleConstants particleData);

private:
    UploadBuffer<ParticleConstants>* m_ParticlesDataBuffer = nullptr;
    bool isInitialized = false;
    uint32_t CBVIndex = -1;
};
}
