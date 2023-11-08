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
        CollisionLayer = 1;
        CollisionMask = 1;
    }
    Vector3 Center = Vector3::Zero; // Relative to game object
    float Radius = 0.f; // In meters
    unsigned char CollisionLayer = 1; // The layer this collider is on
    unsigned char CollisionMask = 1; // The layers this collider collides with

    // Calculate the position of the collider in world space
    inline Vector3 WorldPosition() { return pGameObject->Position() + Center; }
    // Returns true if the collider collides with the other collider
    inline bool CanCollidesWith(SphereCollider& other) { return (CollisionLayer & other.CollisionMask) != 0; }
};
}
