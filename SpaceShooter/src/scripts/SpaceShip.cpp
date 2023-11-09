#include "SpaceShip.h"
#include "Bullet.h"
#include "Blaster/Blaster.h"
#include "Asteroid.h"
using namespace DXH;

void SpaceShip::Start()
{
    m_SpaceshipRigibody = &pGameObject->Get<RigidBody>();
    m_Camera = &pGameObject->Get<Camera>();
    m_IsDead = false;

    m_LeftCannon = new Blaster();
    m_RightCannon = new Blaster();
}

void SpaceShip::Update(const DXH::Timer& gt)
{
    //WSAD for displacement && SPACE to Stop
    Vector3 up, right, forward;
    pGameObject->GetLocalAxis(up, right, forward);
    if (InputManager::GetKeyState('W') == KeyState::Pressed || InputManager::GetKeyState('Z') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity += forward * (m_Propulsion * gt.DeltaTime());
    if (InputManager::GetKeyState('S') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity -= forward * (m_Propulsion * gt.DeltaTime());
    if (InputManager::GetKeyState('A') == KeyState::Pressed || InputManager::GetKeyState('Q') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity -= right * (m_Propulsion * gt.DeltaTime());
    if (InputManager::GetKeyState('D') == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity += right * (m_Propulsion * gt.DeltaTime());
    if (InputManager::GetKeyState(VK_SPACE) == KeyState::Pressed)
        m_SpaceshipRigibody->Velocity -= Vector3(m_SpaceshipRigibody->Velocity.Normalize()) * (m_Propulsion * gt.DeltaTime());

    auto loadedVelocity = m_SpaceshipRigibody->Velocity.Load();
    if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(loadedVelocity)) > m_MaxVelocity * m_MaxVelocity)
    {
        m_SpaceshipRigibody->Velocity.Store(DirectX::XMVector3Normalize(loadedVelocity));
        m_SpaceshipRigibody->Velocity *= m_MaxVelocity;
    }

    if (InputManager::GetKeyState(VK_TAB) == KeyState::JustPressed)
    {
        m_LeftCannon->SwitchWeapon();
        m_RightCannon->SwitchWeapon();
    }

    // Attack
    m_LeftCannon->Update(gt);
    if (InputManager::GetKeyState(VK_LBUTTON) == KeyState::Pressed)
        m_LeftCannon->Shoot(pGameObject->Position() + Vector3::Cross(m_Camera->Forward, up), m_Camera->Forward);

    m_RightCannon->Update(gt);
    if (InputManager::GetKeyState(VK_RBUTTON) == KeyState::Pressed)
        m_RightCannon->Shoot(pGameObject->Position() + Vector3::Cross(up, m_Camera->Forward), m_Camera->Forward);

    // Camera Zoom
    m_Camera->FieldOfView = InputManager::GetKeyState('C') == KeyState::Pressed ? m_CameraDefaultPOV * m_ZoomScale : m_CameraDefaultPOV;
}

void SpaceShip::OnDestroy()
{
    delete m_LeftCannon;
    delete m_RightCannon;
}

void SpaceShip::OnCollision(DXH::GameObject* other)
{
    m_Life--;
    if (m_Life <= 0)
    {
        m_IsDead = true;
    }
}
