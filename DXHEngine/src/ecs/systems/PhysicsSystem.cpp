#include "PhysicsSystem.h"
#include "src/ecs/components/Physics.h"
#include "src/ecs/GameObject.h"
#include "src/time/Timer.h"
using namespace DirectX;

namespace DXH
{


void PhysicsSystem::Update(const Timer& gt)
{
    ResolveCollisions();
    UpdateRigidBodies(gt);
}

// Calculate the collision normal between two positions, from A to B
inline XMVECTOR CalculateCollisionNormal(FXMVECTOR posA, FXMVECTOR posB) { return XMVector3Normalize(posB - posA); }
// Calculate the squared distance between two positions
inline float SqDistanceBetween(FXMVECTOR posA, FXMVECTOR posB) { return XMVectorGetX(XMVector3LengthSq(posB - posA)); }

void PhysicsSystem::ResolveCollisions()
{
    auto& collMap = ComponentManager<SphereCollider>::GetInstance().GetUsedComponentsMap();
    auto& rigidMap = ComponentManager<RigidBody>::GetInstance().GetUsedComponentsMap();

    for (auto it = collMap.begin(); it != collMap.end(); it++)
    {
        GameObject* gameObjectA = it->first;
        SphereCollider& colliderA = it->second;
        XMVECTOR posA = colliderA.WorldPosition().Load();

        for (auto it2 = std::next(it); it2 != collMap.end(); it2++)
        {
            GameObject* gameObjectB = it2->first;
            SphereCollider& colliderB = it2->second;

            // Check for mask and distance
            if (!(colliderA.CanCollidesWith(colliderB)
                && colliderB.CanCollidesWith(colliderA)
                && gameObjectA->IsNear(*gameObjectB))) continue;

            XMVECTOR posB = colliderB.WorldPosition().Load();

            // Check for collision
            float sqDistance = SqDistanceBetween(posA, posB);
            float sumRadii = colliderA.Radius + colliderB.Radius;

            if (sqDistance < sumRadii * sumRadii)
            {
                RigidBody& rigidBodyA = rigidMap.at(gameObjectA);
                RigidBody& rigidBodyB = rigidMap.at(gameObjectB);

                XMVECTOR normal = CalculateCollisionNormal(posA, posB);
                XMVECTOR rbvA = rigidBodyA.Velocity.Load();
                XMVECTOR rbvB = rigidBodyB.Velocity.Load();
                float impulse = (2 * rigidBodyB.Mass * XMVectorGetX(XMVector3Dot(normal, rbvB - rbvA))) /
                    (rigidBodyA.Mass + rigidBodyB.Mass);

                // Update the positions to does not collides anymore
                XMVECTOR penetrationOverMasses = (sumRadii - XMVectorGetX(XMVector3Length(posB - posA))) / (rigidBodyA.Mass + rigidBodyB.Mass) * normal;
                gameObjectA->SetPosition(gameObjectA->Position().Load() - (penetrationOverMasses * rigidBodyA.Mass));
                gameObjectB->SetPosition(gameObjectA->Position().Load() + (penetrationOverMasses * rigidBodyB.Mass));

                // Update the velocities
                rigidBodyA.Velocity.Store(rbvA + (impulse * normal / rigidBodyA.Mass));
                rigidBodyB.Velocity.Store(rbvB - (impulse * normal / rigidBodyB.Mass));
            }
        }
    }
}

void PhysicsSystem::UpdateRigidBodies(const Timer& gt)
{
    for (auto& [gameObject, rigidBody] : ComponentManager<RigidBody>::GetInstance().GetUsedComponentsMap())
    {
        // Apply the velocity to the position
        gameObject->SetPosition(rigidBody.Velocity.Load() * (gt.DeltaTime() * gt.TimeScale()) + gameObject->Position().Load());
    }
}
}