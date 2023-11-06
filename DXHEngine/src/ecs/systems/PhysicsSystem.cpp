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
    int c = 0;
    auto& map = ComponentManager<SphereCollider>::GetInstance().GetUsedComponentsMap();
    for (auto it = map.begin(); it != map.end(); it++)
    {
        const GameObject* gameObjectA = it->first;
        Transform& transformA = gameObjectA->Get<Transform>();
        SphereCollider& colliderA = it->second;
        XMVECTOR posA = ColliderPosition(transformA, colliderA);

        for (auto it2 = std::next(it); it2 != map.end(); it2++)
        {
            const GameObject* gameObjectB = it2->first;
            Transform& transformB = gameObjectB->Get<Transform>();
            SphereCollider& colliderB = it2->second;
            XMVECTOR posB = ColliderPosition(transformB, colliderB);
            
            // Check for grid position 
            if (transformA.GridPosition.x > transformB.GridPosition.x + 1 || transformA.GridPosition.x < transformB.GridPosition.x - 1 || 
                transformA.GridPosition.y > transformB.GridPosition.y + 1 || transformA.GridPosition.y < transformB.GridPosition.y - 1 ||
                transformA.GridPosition.z > transformB.GridPosition.z + 1 || transformA.GridPosition.z < transformB.GridPosition.z - 1) continue;
            

            // Check for collision
            float sqDistance = SqDistanceBetween(posA, posB);
            float sumRadii = colliderA.Radius + colliderB.Radius;

            if (sqDistance < sumRadii * sumRadii)
            {
                RigidBody& rigidBodyA = gameObjectA->Get<RigidBody>();
                RigidBody& rigidBodyB = gameObjectB->Get<RigidBody>();
            
                XMVECTOR normal = CalculateCollisionNormal(posA, posB);
                XMVECTOR relativeVelocity = rigidBodyB.Velocity.Load() - rigidBodyA.Velocity.Load();
                float impulse = (2 * rigidBodyB.Mass * XMVectorGetX(XMVector3Dot(normal, relativeVelocity))) /
                    (rigidBodyA.Mass + rigidBodyB.Mass);
            
                // Update the velocities
                rigidBodyA.Velocity.Store(rigidBodyA.Velocity.Load() + (impulse * normal / rigidBodyA.Mass));
                rigidBodyB.Velocity.Store(rigidBodyB.Velocity.Load() - (impulse * normal / rigidBodyB.Mass));
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
        transform.SetPosition(rigidBody.Velocity.Load() * gt.DeltaTime() + transform.Position.Load());
    }
}
}