#pragma once
#include "src/ecs/Component.h"

namespace DXH
{
struct RigidBody : Component
{
	RigidBody(Entity* entity) : Component(entity) {}
	Vector3 velociy = {0,0,0};
	Vector3 force = {0,0,0};
	float mass = 1.0f;
};
}
