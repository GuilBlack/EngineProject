#pragma once
#include "src/ecs/Entity.h"

namespace DXH
{
struct RigidBody : Component
{
	RigidBody(Entity* entity) : Component(entity) {}
	Vector3 Velociy = {0,0,0};
	Vector3 Force = {0,0,0};
	float Mass = 1.0f;
};

struct SphereCollider : Component
{
	SphereCollider(Entity* entity) : Component(entity) {}
	Vector3 Center = { 0,0,0 };
	float Radius = 0.f;
};
}
