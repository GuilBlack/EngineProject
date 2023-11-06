#pragma once
#include <DXHScripting.h>

class SpaceShip : public DXH::Script
{
public:
    void Start() override;
    void Update(const DXH::Timer& gt) override;
private:
    DXH::RigidBody* m_SpaceshipRigibody;
    float m_DefaultSpeed = 1.f;
};
