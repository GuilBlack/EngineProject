#pragma once
#include "src/ecs/Component.h" // For the Component base class

namespace DXH
{
/// <summary>
/// Rigid body for application of force and velocity.
/// </summary>
struct RigidBody : Component
{
    void OnAssign() override
    {
        Velocity = Vector3::Zero;
        Force = Vector3::Zero;
        Mass = 1.0f;
    }
    Vector3 Velocity;
    Vector3 Force;
    float Mass;
};
/// <summary>
/// A sphere collider.
/// </summary>
struct SphereCollider : Component
{
    void OnAssign() override
    {
        Center = Vector3::Zero;
        Radius = 0.f;
    }
    Vector3 Center; // Relative to game object
    float Radius;
};
}
