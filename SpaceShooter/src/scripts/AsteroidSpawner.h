#pragma once
#include <DXHScripting.h>

class AsteroidSpawner : public DXH::Script
{
public:
	void Update(const DXH::Timer& gt) override;
	void CreateAsteroid(DXH::Vector3& point);
	void SetSpaceShip(DXH::GameObject* spaceShip) { m_SpaceShip = spaceShip; }
	void ResetAsteroidSpawner();

private:
	float m_Radius = 20.f; 
	float m_SpawnRate = 4.f;
	DXH::GameObject* m_SpaceShip = nullptr;
	float m_DeltaTime = 0;
	float m_AsteroidSpeed = 3.f;
};

