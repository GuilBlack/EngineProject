#include "AsteroidSpawner.h"
#include "Asteroid.h"
#include "SpaceShip.h"

using namespace DXH;
void AsteroidSpawner::Update(const DXH::Timer& gt)
{
    if(m_SpaceShip->Get<SpaceShip>().IsSpaceShipDead() == true)
		ResetAsteroidSpawner();

    if(m_SpaceShip == nullptr)
		return;

    m_DeltaTime += gt.DeltaTime();
    if (m_DeltaTime >= m_SpawnRate)
    {
        m_DeltaTime -= 1.f;
        auto random = [](float minVal, float maxVal) -> float
            {
                return (float)rand() / RAND_MAX * (maxVal - minVal) + minVal;
            };


        float pLat = random(0.f, DirectX::XM_2PI);
        float pLong = random(0.f, DirectX::XM_2PI);
        Vector3 point =
        {
            sinf(pLat) * cosf(pLong) * m_Radius,
            cosf(pLat) * m_Radius,
            sinf(pLat) * sinf(pLong) * m_Radius
        };

        CreateAsteroid(point);
        m_SpawnRate -= 0.1f;
    }
}

void AsteroidSpawner::CreateAsteroid(Vector3& point)
{
    GameObject* pAsteroid = GameObject::Create();
    pAsteroid->Add<Mesh>().SetGeoAndMatByName("Sphere", "AsteroidMaterial");
    auto& rb = pAsteroid->Add<RigidBody>();

    pAsteroid->Add<SphereCollider>().Radius = 1.f;
    auto& a = pAsteroid->Add<Asteroid>();
    pAsteroid->SetPosition(point + m_SpaceShip->Position());     
    Vector3 normalize = (m_SpaceShip->Position() - pAsteroid->Position()).Normalize();
    rb.Velocity = normalize * m_AsteroidSpeed;
    a.SetSpaceShip(m_SpaceShip);
    a.SetAsteroidSpeed(m_AsteroidSpeed);
}

void AsteroidSpawner::ResetAsteroidSpawner()
{
    m_SpawnRate = 4.f; 
    m_DeltaTime = 0.f;
}
