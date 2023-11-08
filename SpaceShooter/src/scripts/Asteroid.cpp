#include "Asteroid.h"
using namespace DXH;

void Asteroid::Update(const DXH::Timer& gt)
{
    //TODO
}

Asteroid& Asteroid::CreateAsteroid()
{
    GameObject* pAsteroid = GameObject::Create();
    pAsteroid->Add<Mesh>().SetGeoAndMatByName("Sphere", "AsteroidMaterial");
    pAsteroid->Add<RigidBody>();
    pAsteroid->Add<SphereCollider>().Radius = 1.f;
    return pAsteroid->Add<Asteroid>();
}

void Asteroid::SetRandomPosition()
{
    float randX = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
    float randY = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
    float randZ = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
    pGameObject->SetPosition(randX, randY, randZ);
}
