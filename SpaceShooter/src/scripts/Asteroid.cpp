#include "Asteroid.h"
using namespace DXH;

void Asteroid::Update(const DXH::Timer& gt)
{
	//TODO
}

GameObject* Asteroid::CreateAsteroid()
{
	GameObject* asteroid = GameObject::Create();
	asteroid->Add<RigidBody>();
	auto& asteroidCollider = asteroid->Add<SphereCollider>();
	asteroidCollider.Radius = 0.1f;
	asteroid->Add<Mesh>().SetGeoAndMatByName("Sphere", "AsteroidMaterial");
	asteroid->Add<Asteroid>().SetRandomPosition();
	return asteroid;
}

void Asteroid::SetRandomPosition()
{
	float randX = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
	float randY = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
	float randZ = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
	pGameObject->SetPosition(randX, randY, randZ);
}
