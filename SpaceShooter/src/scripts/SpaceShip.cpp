#include "SpaceShip.h"
#include "../Game.h"
using namespace DXH;

void SpaceShip::Start()
{
    m_SpaceshipRigibody = &pGameObject->Get<RigidBody>();
    m_RuntimeBullet = std::vector<GameObject*>();
}

void SpaceShip::Update(const DXH::Timer& gt)
{
    Vector3 up, right, forward;
    pGameObject->GetLocalAxis(up, right, forward);
    if (InputManager::IsKeyPressed('W'))
        m_SpaceshipRigibody->Velocity += forward * m_DefaultSpeed;
    if (InputManager::IsKeyPressed('S'))
        m_SpaceshipRigibody->Velocity -= forward * m_DefaultSpeed;
    if (InputManager::IsKeyPressed('A'))
        m_SpaceshipRigibody->Velocity -= right * m_DefaultSpeed;
    if (InputManager::IsKeyPressed('D'))
        m_SpaceshipRigibody->Velocity += right * m_DefaultSpeed;

    if (InputManager::IsKeyPressed(VK_LBUTTON))
        CreateBulletGO();
    VS_DB_OUT_A("In scene Game Object : " << m_RuntimeBullet.size() << std::endl);

    auto loadedVelocity = m_SpaceshipRigibody->Velocity.Load();
    if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(loadedVelocity)) > m_SqMaxVelocity)
    {
        m_SpaceshipRigibody->Velocity.Store(DirectX::XMVector3Normalize(loadedVelocity));
        m_SpaceshipRigibody->Velocity *= m_SqMaxVelocity;
    }
}

void SpaceShip::OnDestroy()
{
    for (auto go : m_RuntimeBullet)
    {
        delete go;
    }
}

void SpaceShip::CreateBulletGO()
{
	GameObject* bullet = new GameObject();
	bullet->Add<RigidBody>().Mass = 0.5f;
	bullet->Add<SphereCollider>().Radius = 1.f;
    m_RuntimeBullet.emplace_back(bullet);
}