#include "SpaceShip.h"
#include "Bullet.h"
using namespace DXH;

void SpaceShip::Start()
{
    m_SpaceshipRigibody = &pGameObject->Get<RigidBody>();
    m_Camera = &pGameObject->Get<Camera>();
}

void SpaceShip::Update(const DXH::Timer& gt)
{
    //WSAD for displacement && SPACE to Stop
    Vector3 up, right, forward;
    pGameObject->GetLocalAxis(up, right, forward);
    if (InputManager::GetKeyState('W') == KeyState::Pressed || InputManager::GetKeyState('Z') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity += forward * m_DefaultSpeed;
    if (InputManager::GetKeyState('S') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity -= forward * m_DefaultSpeed;
    if (InputManager::GetKeyState('A') == KeyState::Pressed || InputManager::GetKeyState('Q') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity -= right * m_DefaultSpeed;
    if (InputManager::GetKeyState('D') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity += right * m_DefaultSpeed;
    if (InputManager::GetKeyState(VK_SPACE) == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity = Vector3::Zero;

    auto loadedVelocity = m_SpaceshipRigibody->Velocity.Load();
    if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(loadedVelocity)) > m_SqMaxVelocity)
    {
        m_SpaceshipRigibody->Velocity.Store(DirectX::XMVector3Normalize(loadedVelocity));
        m_SpaceshipRigibody->Velocity *= m_SqMaxVelocity;
    }
    m_FireCooldown -= gt.DeltaTime();
    if (m_FireCooldown <= 0 && InputManager::GetKeyState(VK_LBUTTON) == KeyState::Pressed)
    {
        float sideOffset = m_IsLeft ? -1.f : 1.f;
        Bullet::CreateNShoot(pGameObject->Position()+ m_Camera->Forward + Vector3(sideOffset, 0.f, 0.f) , m_Camera->Forward, 3.f);
        m_FireCooldown = m_FireRate;
        m_IsLeft = !m_IsLeft;
    }
    //Camera Zoom
    if (InputManager::GetKeyState('C') == KeyState::JustPressed)
        m_Camera->FieldOfView *= m_ZoomScale;
    if (InputManager::GetKeyState('C') == KeyState::JustReleased)
        m_Camera->FieldOfView = m_CameraDefaultPOV;
}
