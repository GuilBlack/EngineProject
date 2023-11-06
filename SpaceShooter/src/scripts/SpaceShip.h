#pragma once
#include <DXHScripting.h>

class SpaceShip : public DXH::Script
{
public:
    void Start() override;
    void Update(const DXH::Timer& gt) override;
    void OnDestroy() override;
private:
    void CreateBulletGO();
    void Shoot();
    DXH::RigidBody* m_SpaceshipRigibody;
    DXH::GameObject* m_BulletPrefab;
    std::vector<DXH::GameObject*> m_RuntimeBullet;
    float m_DefaultSpeed = 1.f;
    float m_SqMaxVelocity = 10.f;
};
