#include "SpaceShip.h"
#include "Bullet.h"
using namespace DXH;

void SpaceShip::Start()
{
    m_SpaceshipRigibody = &pGameObject->Get<RigidBody>();
}

void SpaceShip::Update(const DXH::Timer& gt)
{
    Vector3 up, right, forward;
    pGameObject->GetLocalAxis(up, right, forward);
    if (InputManager::GetKeyState('W') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity += forward * m_DefaultSpeed;
    if (InputManager::GetKeyState('S') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity -= forward * m_DefaultSpeed;
    if (InputManager::GetKeyState('A') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity -= right * m_DefaultSpeed;
    if (InputManager::GetKeyState('D') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity += right * m_DefaultSpeed;

    auto loadedVelocity = m_SpaceshipRigibody->Velocity.Load();
    if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(loadedVelocity)) > m_SqMaxVelocity)
    {
        m_SpaceshipRigibody->Velocity.Store(DirectX::XMVector3Normalize(loadedVelocity));
        m_SpaceshipRigibody->Velocity *= m_SqMaxVelocity;
    }

    if (InputManager::IsKeyPressed(VK_LBUTTON))
        Bullet::CreateNShoot(pGameObject->Position(), forward, 3.f);
}
