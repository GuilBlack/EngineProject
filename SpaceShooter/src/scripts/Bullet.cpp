#include "Bullet.h"
using namespace DXH;

void Bullet::Update(const Timer& gt)
{
    m_LifeTime -= gt.DeltaTime();
    if (m_LifeTime <= 0.f)
        pGameObject->Destroy();
}

GameObject* Bullet::CreateNShoot(Vector3 position, Vector3 direction, float lifetime)
{
    direction = direction.Normalize();
    GameObject* bullet = GameObject::Create();
    bullet->Add<Bullet>().SetLifeTime(lifetime);
    bullet->Add<RigidBody>().Velocity = direction;
    bullet->Add<SphereCollider>().Radius = 0.1f;
    bullet->Add<Mesh>().SetGeoAndMatByName("Sphere", "AsteroidMaterial");
    bullet->SetPosition(position + direction * 2.f);
    bullet->SetScale(0.1f, 0.1f, 0.1f);
    return bullet;
}
