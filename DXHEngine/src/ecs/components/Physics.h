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
/// Collision layer constants.
/// </summary>
namespace CollisionLayer
{
const unsigned char None = 0b00000000;
const unsigned char Default = 0b00000001;
const unsigned char One = 0b00000010;
const unsigned char Two = 0b00000100;
const unsigned char Three = 0b00001000;
const unsigned char Four = 0b00010000;
const unsigned char Five = 0b00100000;
const unsigned char Six = 0b01000000;
const unsigned char Seven = 0b10000000;
const unsigned char All = 0b11111111;
}
/// <summary>
/// A sphere collider.
/// </summary>
struct SphereCollider : Component
{
    void OnAssign() override
    {
        Center = Vector3::Zero;
        Radius = 0.f;
        CollisionLayer = CollisionLayer::Default;
        CollisionMask = CollisionLayer::Default;
    }
    Vector3 Center = Vector3::Zero; // Relative to game object
    float Radius = 0.f; // In meters
    unsigned char CollisionLayer = CollisionLayer::Default; // The layer this collider is on
    unsigned char CollisionMask = CollisionLayer::Default; // The layers this collider can collide with

    // Calculate the position of the collider in world space
    inline Vector3 WorldPosition() { return pGameObject->Position() + Center; }
    // Returns true if the collider collides with the other collider
    inline bool CanCollidesWith(SphereCollider& other) { return (CollisionLayer & other.CollisionMask) != 0; }
};
}
