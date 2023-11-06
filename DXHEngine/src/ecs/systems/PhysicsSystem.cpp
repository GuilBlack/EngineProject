#include "PhysicsSystem.h"
#include "src/ecs/components/Transform.h"
#include "src/ecs/components/Physics.h"
#include "src/ecs/GameObject.h"
#include "src/time/Timer.h"
using namespace DirectX;

namespace DXH
{


void PhysicsSystem::Update(const Timer& gt)
{
    Timer t; 
    t.Tick();
    ResolveCollisions(gt);
    t.Tick();
    VS_DB_OUT_A("Resolve Collision: " << t.DeltaTime() * 1000 << "ms\n");
    t.Tick();
    UpdateRigidBodies(gt);
    t.Tick();
    VS_DB_OUT_A("Update Rigidbodies: " << t.DeltaTime() * 1000 << "ms\n");
}

// Calculate the position of the collider in world space
inline XMVECTOR ColliderPosition(Transform& transform, SphereCollider& collider) { return transform.Position.Load() + collider.Center.Load(); }
// Calculate the dot product between two vectors
inline float SqDistanceBetween(FXMVECTOR& posA, FXMVECTOR& posB) { return XMVectorGetX(XMVector3LengthSq(posB - posA)); }
// Calculate the collision normal between two positions, from A to B
inline XMVECTOR CalculateCollisionNormal(FXMVECTOR& posA, FXMVECTOR& posB) { return XMVector3Normalize(posB - posA); }

void PhysicsSystem::ResolveCollisions(const Timer& gt)
{
    auto& collMap = ComponentManager<SphereCollider>::GetInstance().GetUsedComponentsMap();
    auto& rigidMap = ComponentManager<RigidBody>::GetInstance().GetUsedComponentsMap();
    auto& transformMap = ComponentManager<Transform>::GetInstance().GetUsedComponentsMap();

    for (auto it = collMap.begin(); it != collMap.end(); it++)
    {
        const GameObject* gameObjectA = it->first;
        Transform& transformA = transformMap.at(gameObjectA);
        SphereCollider& colliderA = it->second;
        XMVECTOR posA = ColliderPosition(transformA, colliderA);

        for (auto it2 = std::next(it); it2 != collMap.end(); it2++)
        {
            const GameObject* gameObjectB = it2->first;
            Transform& transformB = transformMap.at(gameObjectB);

            // Check for grid position 
            if (abs(transformA.GridPosition[0] - transformB.GridPosition[0]) >= 2 ||
                abs(transformA.GridPosition[1] - transformB.GridPosition[1]) >= 2 || 
                abs(transformA.GridPosition[2] - transformB.GridPosition[2] >= 2)) continue;

            SphereCollider& colliderB = it2->second;
            XMVECTOR posB = ColliderPosition(transformB, colliderB);           

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
                transformA.SetPosition(transformA.Position.Load() - (penetrationOverMasses * rigidBodyA.Mass));
                transformB.SetPosition(transformB.Position.Load() + (penetrationOverMasses * rigidBodyB.Mass));

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
        Transform& transform = gameObject->Get<Transform>();
        transform.SetPosition(rigidBody.Velocity.Load() * (gt.DeltaTime() * gt.TimeScale()) + transform.Position.Load());
    }
}
}