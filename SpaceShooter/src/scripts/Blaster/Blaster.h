#pragma once
#include <DXHScripting.h>
#include "MachineGun.h"
#include "Sniper.h"

class Blaster : public DXH::StateMachine
{
public:
    Blaster() : DXH::StateMachine()
    {
        m_MachineGun = MachineGun();
        m_Sniper = Sniper();
        m_currentState = &m_MachineGun;
    }

    void Shoot(DXH::Vector3 position, DXH::Vector3 direction)
    {
        static_cast<Weapon*>(m_currentState)->Shoot(position, direction);
    }

    void SwitchToMachineGun() { SwitchState(&m_MachineGun); }
    void SwitchToSniper() { SwitchState(&m_Sniper); }
    void SwitchWeapon()
    {
        if (m_IsMachineGun)
            SwitchToSniper();
        else
            SwitchToMachineGun();
        m_IsMachineGun = !m_IsMachineGun;
    }

private:
    MachineGun m_MachineGun;
    Sniper m_Sniper;
    bool m_IsMachineGun = true;
};