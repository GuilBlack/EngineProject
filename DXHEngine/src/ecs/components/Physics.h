#pragma once
#include "src/ecs/Component.h"

namespace DXH
{
/// <summary>
/// Rigid body for application of force and velocity.
/// </summary>
struct RigidBody : Component
{
	void Reset() override
	{
		Velociy = {0,0,0};
		Force = {0,0,0};
		Mass = 1.0f;
	}
	Vector3 Velociy;
	Vector3 Force;
	float Mass;
};
/// <summary>
/// A sphere collider.
/// </summary>
struct SphereCollider : Component
{
	void Reset() override
	{
		Center = {0,0,0};
		Radius = 0.f;
	}
	Vector3 Center; // Relative to game object
	float Radius;
};
}
