#pragma once
#include <DXHScripting.h>

class Bullet : public DXH::Script
{
public:
    void Update(const DXH::Timer& gt) override;

    static DXH::GameObject* CreateNShoot(DXH::Vector3 position, DXH::Vector3 velocity, float lifetime, float damage);
    void SetLifeTime(float lifetime) { m_LifeTime = lifetime; }
    void OnCollision(DXH::GameObject* other) override;

private:
    float m_LifeTime = 1.f;
    float m_Damage = 1.f;
};
