#include "Asteroid.h"
using namespace DXH;

void Asteroid::Update(const DXH::Timer& gt)
{
    ReplaceAsteroid();
}

Asteroid& Asteroid::CreateAsteroid(GameObject* SpaceShip)
{
    GameObject* pAsteroid = GameObject::Create();
    pAsteroid->Add<Mesh>().SetGeoAndMatByName("Sphere", "AsteroidMaterial");
    pAsteroid->Add<RigidBody>();
    pAsteroid->Add<SphereCollider>().Radius = 1.f;
    auto &a = pAsteroid->Add<Asteroid>();
    a.m_SpaceShip = SpaceShip;
    return a;
}

void Asteroid::SetRandomPosition()
{
    float randX = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
    float randY = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
    float randZ = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
    pGameObject->SetPosition(randX, randY, randZ);
}

void Asteroid::ReplaceAsteroid()
{
    auto astePos = pGameObject->Position();
    auto playerPosition = m_SpaceShip->Position();
    Vector3 calculation = astePos - playerPosition;
    calculation.x = calculation.x * calculation.x;
    calculation.y = calculation.y * calculation.y;
    calculation.z = calculation.z * calculation.z;
    float distance = sqrt(calculation.x + calculation.y + calculation.z);

    if (distance > 100.f)
    {
        pGameObject->Get<RigidBody>().Velocity = Vector3(0.f, 0.f, 0.f);
        SetRandomPosition();
    }
}
