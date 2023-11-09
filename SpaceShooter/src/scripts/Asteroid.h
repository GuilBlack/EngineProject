#pragma once
#include <DXHScripting.h>

class Asteroid : public DXH::Script
{
public:
    void Update(const DXH::Timer& gt) override;
    void OnAsteroidDestroy();
    void OnCollision(DXH::GameObject* other) override;

    void SetSpaceShip(DXH::GameObject* ss) { m_SpaceShip = ss; }
    void SetAsteroidSpeed(float speed) { m_AsteroidSpeed = speed; }

private:
    DXH::GameObject* m_SpaceShip = nullptr;
    bool m_IsDestroyed = false;
    float m_TimeToLive = 0.f;
    float m_AsteroidSpeed = 0.f;
};

