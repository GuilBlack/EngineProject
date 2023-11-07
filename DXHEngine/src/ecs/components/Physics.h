#pragma once
#include "src/ecs/Component.h" // For the Component base class
#include "DXHMaths.h" // For the Vector3 and DirectXMath types
#include "src/ecs/GameObject.h" // For the GameObject position

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
        Mass = 1.0f;
    }
    Vector3 Velocity = Vector3::Zero; // In meters per second
    float Mass = 1.0f; // In kilograms
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
    Vector3 Center = Vector3::Zero; // Relative to game object
    float Radius = 0.f;

    // Calculate the position of the collider in world space
    inline Vector3 WorldPosition() { return pGameObject->Position() + Center; }
};
}
