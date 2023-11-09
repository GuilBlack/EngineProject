#pragma once
#include "Weapon.h"

class MachineGun : public Weapon
{
public:
    MachineGun() : Weapon()
    {
        m_FireRate = 10.f;
        m_ProjectileSpeed = 10.f;
        m_LifeTime = 5.f;
        m_ProjectileDamage = 1.f;
    }
    ~MachineGun() = default;
};
