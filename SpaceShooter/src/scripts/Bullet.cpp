#include "Bullet.h"
using namespace DXH;

void Bullet::Update(const Timer& gt)
{
    m_LifeTime -= gt.DeltaTime();
    if (m_LifeTime <= 0.f)
        pGameObject->Destroy();
}

DXH::GameObject* Bullet::CreateNShoot(DXH::Vector3 position, DXH::Vector3 velocity, float lifetime, float damage)
{
    GameObject* bullet = GameObject::Create();
    auto& bs = bullet->Add<Bullet>();
    bs.SetLifeTime(lifetime);
    bs.m_Damage = damage;
    bullet->Add<RigidBody>().Velocity = velocity;
    bullet->Add<SphereCollider>().Radius = 0.1f;
    bullet->Add<Mesh>().SetGeoAndMatByName("Sphere", "AsteroidMaterial");
    bullet->SetPosition(position + velocity.Normalize());
    bullet->SetScale(0.1f, 0.1f, 0.1f);
    return bullet;
}

void Bullet::OnCollision(DXH::GameObject* other)
{
    pGameObject->Destroy();
}
