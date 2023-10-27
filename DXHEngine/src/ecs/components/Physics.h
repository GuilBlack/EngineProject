#pragma once
#include "src/ecs/Entity.h"

namespace DXH
{
/// <summary>
/// Rigid body for application of force and velocity.
/// </summary>
struct RigidBody : Component
{
	RigidBody(Entity* entity) : Component(entity) {}
	Vector3 Velociy = {0,0,0};
	Vector3 Force = {0,0,0};
	float Mass = 1.0f;
};
/// <summary>
/// A sphere collider. The entity must have a rigid body.
/// </summary>
struct SphereCollider : Component
{
	SphereCollider(Entity* entity) : Component(entity)
	{
		assert(entity->HasComponent<RigidBody>()); // sphere collider must have rigid body
	}
	Vector3 Center = {0,0,0}; // relative to entity
	float Radius = 0.f;
};
/// <summary>
/// Represents a collision between two entities.
/// </summary>
struct Collision
{
	SphereCollider* First = nullptr;
	SphereCollider* Second = nullptr;
	Vector3 Normal = {0,0,0}; // Normal of the collision, pointing from first to second
};
}
