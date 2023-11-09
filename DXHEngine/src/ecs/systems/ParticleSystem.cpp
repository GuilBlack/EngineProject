#include "ParticleSystem.h"
#include "src/time/Timer.h"
#include "../components/Particles.h"
#include "../ComponentManager.h"
#include "src/renderer/Renderer.h"
#include "src/renderer/ConstantBuffers.h"

namespace DXH
{
void ParticleSystem::Update(const Timer& gt)
{
    auto& particlesMap = ComponentManager<Particles>::GetInstance().GetUsedComponentsMap();

    for (auto& pair : particlesMap)
    {
        GameObject* go = pair.first;
        Particles& particles = pair.second;
        float deltaTime = gt.DeltaTime();
        int i = 0;
        for (auto& particle : particles.ParticlesData)
        {
            if (particle.TimeAlive < particle.MaxTimeAlive)
            {
                particle.TimeAlive += deltaTime;
                particle.Pos = DirectX::XMVectorAdd(
                    particle.Pos.Load(),
                    DirectX::XMVectorScale(particle.Velocity.Load(), gt.DeltaTime())
                );

                ParticleConstants pc =
                {
                    .World = DirectX::XMMatrixTranspose(DirectX::XMMatrixScalingFromVector(particle.Scale.Load()) * DirectX::XMMatrixTranslationFromVector(particle.Pos.Load())),
                    .BeginColor = particle.BeginColor,
                    .EndColor = particle.EndColor,
                    .TTLRatio = particle.TimeAlive / particle.MaxTimeAlive,
                };

                particles.UpdateInstanceData(i, pc);
            }
            ++i;
        }
    }
}
}
