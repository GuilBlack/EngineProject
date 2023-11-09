#pragma once
#include <DXHScripting.h>
#include "../Bullet.h"

class Weapon : public DXH::BaseState
{
public:
    Weapon() = default;
    ~Weapon() = default;

    void EnterState() override
    {
        m_CoolDown = 0.f;
    }
    void LeaveState() override {}

    void Shoot(DXH::Vector3 position, DXH::Vector3 direction)
    {
        if (m_CoolDown <= 0.f)
        {
            Bullet::CreateNShoot(position, DXH::Vector3(direction.Normalize()) * m_ProjectileSpeed, m_LifeTime, m_ProjectileDamage);
            m_CoolDown = 1.f / m_FireRate;
        }
    }

    void Update(const DXH::Timer& gt) override
    {
        if (m_CoolDown > 0.f)
            m_CoolDown -= gt.DeltaTime();
    }

protected:
    float m_FireRate; // bullets per second
    float m_ProjectileSpeed;
    float m_LifeTime;
    float m_ProjectileDamage;

private:
    float m_CoolDown = 0.f;
};
