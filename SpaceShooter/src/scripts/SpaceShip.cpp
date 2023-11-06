#include "SpaceShip.h"

using namespace DXH;

void SpaceShip::Start()
{
    m_SpaceshipRigibody = &pGameObject->Get<RigidBody>();
}

void SpaceShip::Update(const DXH::Timer& gt)
{
    if (InputManager::IsKeyPressed('W'))
        m_SpaceshipRigibody->Velocity.z += m_DefaultSpeed;
    if (InputManager::IsKeyPressed('S'))
        m_SpaceshipRigibody->Velocity.z -= m_DefaultSpeed;
    if (InputManager::IsKeyPressed('A'))
        m_SpaceshipRigibody->Velocity.x -= m_DefaultSpeed;
    if (InputManager::IsKeyPressed('D'))
        m_SpaceshipRigibody->Velocity.x += m_DefaultSpeed;
}

