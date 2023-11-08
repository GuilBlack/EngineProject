#pragma once
#include <DXHScripting.h>

class Asteroid : public DXH::Script
{
public:
	void Update(const DXH::Timer& gt) override;
	static DXH::GameObject* CreateAsteroid();
	void SetRandomPosition();

};

