#pragma once
#include <DXHScripting.h>

class Bullet : public DXH::Script
{
public:
    void Update(const DXH::Timer& gt) override;

    static DXH::GameObject* CreateNShoot(DXH::Vector3 position, DXH::Vector3 velocity, float lifetime);
    void SetLifeTime(float lifetime) { m_LifeTime = lifetime; }

private:
    float m_LifeTime = 1.f;
    float m_BulletSpeed = 15.f;
};