#include "Particles.h"
#include "src/renderer/UploadBuffer.h"
#include "../GameObject.h"
#include "../../renderer/Shader.h"

namespace DXH
{
    void Particles::OnAssign()
    {
        ParticlesData.reserve(100);
        auto random = [](float minVal, float maxVal) -> float
            {
                return (float)rand() / RAND_MAX * (maxVal - minVal) + minVal;
            };

        m_ParticlesDataBuffer = new UploadBuffer<ParticleConstants>(100, false);

        for (uint32_t i = 0; i < 100; ++i)
        {
            ParticleData particle = {
                .Pos = {0.f, 0.f, 0.f},
                .Scale = {.3f, .3f, .3f},
                .Velocity = Vector3(random(-5.f, 5.f), random(-5.f, 5.f), random(-5.f, 5.f)),
                .MaxTimeAlive = random(4.f, 6.f),
                .BeginColor = Color(0.41f, 0.36f, 0.27f, 1.0f),
                .EndColor = Color(0.f, 0.f, 0.f, 0.f)
            };

            ParticleConstants pc =
            {
                .World = DirectX::XMMatrixTranspose(DirectX::XMMatrixScalingFromVector(particle.Scale.Load())*DirectX::XMMatrixTranslationFromVector(particle.Pos.Load())),
                .BeginColor = particle.BeginColor,
                .EndColor = particle.EndColor,
                .TTLRatio = particle.TimeAlive / particle.MaxTimeAlive,
            };

            m_ParticlesDataBuffer->CopyData(i, pc);
            ParticlesData.emplace_back(std::move(particle));
        }
        ParticlesData.resize(100);

        if (!isInitialized)
        {
            isInitialized = true;
            CBVIndex = BaseShader::AddObjectCB();
        }
    }

    void Particles::OnDetach()
    {
        m_ParticlesDataBuffer->Destroy();
        DELETE_PTR(m_ParticlesDataBuffer);
        ParticlesData.clear();
    }

    void Particles::UpdateInstanceData(uint32_t index, ParticleConstants particleData)
    {
        m_ParticlesDataBuffer->CopyData(index, particleData);
    }
}
