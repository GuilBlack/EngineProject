#pragma once
#include "Weapon.h"

class Sniper : public Weapon
{
public:
    Sniper() : Weapon()
    {
        m_FireRate = 1.f;
        m_ProjectileSpeed = 20.f;
        m_LifeTime = 5.f;
        m_ProjectileDamage = 5.f;
    }
    ~Sniper() = default;
};
