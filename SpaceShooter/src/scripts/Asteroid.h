#pragma once
#include <DXHScripting.h>

class Asteroid : public DXH::Script
{
public:
    void Update(const DXH::Timer& gt) override;
    static Asteroid& CreateAsteroid(DXH::GameObject* SpaceShip);
    void SetRandomPosition();
    void ReplaceAsteroid();

private:
    DXH::GameObject* m_SpaceShip = nullptr;
};

