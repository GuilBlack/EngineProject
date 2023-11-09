#pragma once
#include <DXHScripting.h>

class Blaster;

class SpaceShip : public DXH::Script
{
public:
    void Start() override;
    void Update(const DXH::Timer& gt) override;
    void OnDestroy() override;
    void GetPlayerLife(int& life) { life = m_Life; }
    void OnCollision(DXH::GameObject* other) override;
    bool IsSpaceShipDead() { return m_IsDead; }

private:
    DXH::RigidBody* m_SpaceshipRigibody;
    DXH::Camera* m_Camera;
    float m_Propulsion = 2.f;
    float m_MaxVelocity = 5.f;

    float m_CameraDefaultPOV = 65.f;
    float m_ZoomScale = 0.5f;

    Blaster* m_LeftCannon;
    Blaster* m_RightCannon;

    int m_Life = 3;
    bool m_IsDead = false;
};
