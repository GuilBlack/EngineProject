#include "Asteroid.h"
#include "DXHRendering.h"
#include "SpaceShip.h"
using namespace DXH;

void Asteroid::Update(const DXH::Timer& gt)
{
    if (m_SpaceShip->Get<SpaceShip>().IsSpaceShipDead())
        OnAsteroidDestroy();

    auto& rb = pGameObject->Get<RigidBody>();
    Vector3 normalize = (m_SpaceShip->Position() - pGameObject->Position()).Normalize();

    rb.Velocity = (rb.Velocity + normalize).Normalize();
    rb.Velocity *= m_AsteroidSpeed;

    if (!m_IsDestroyed)
		return;

    m_TimeToLive += gt.DeltaTime();

    pGameObject->Remove<SphereCollider>();
	if (m_TimeToLive > 6.f)
        OnAsteroidDestroy();

}

void Asteroid::OnAsteroidDestroy()
{
    pGameObject->Destroy();
}

void Asteroid::OnCollision(DXH::GameObject* other)
{
    pGameObject->Remove<Mesh>();
    if (!pGameObject->Has<Particles>())
    {
        auto& particles = pGameObject->Add<Particles>();
        particles.Geo = RendererResource::GetGeometry("Sphere");
    }
    m_IsDestroyed = true;
    pGameObject->Get<RigidBody>().Velocity = Vector3(0.f, 0.f, 0.f);
}
