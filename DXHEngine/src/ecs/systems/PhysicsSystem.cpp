#include "PhysicsSystem.h"
#include "src/ecs/components/Transform.h"
#include "src/ecs/components/Physics.h"
#include "src/ecs/GameObject.h"
#include "src/core/Timer.h"

namespace DXH
{
PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Update(const Timer& gt)
{
	// Get all GO with a sphere collider
	auto& map = ComponentManager<SphereCollider>::UsedComponentsMap();

	// TODO
}

inline DirectX::XMVECTOR PhysicsSystem::ColliderPosition(Transform* transform, SphereCollider* collider)
{
	return DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&transform->position), DirectX::XMLoadFloat3(&collider->Center));
}

inline float PhysicsSystem::SqrDistanceBetween(DirectX::XMVECTOR posA, DirectX::XMVECTOR posB)
{
	return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(posA, posB)));
}

inline Vector3 PhysicsSystem::CalculateCollisionNormal(DirectX::XMVECTOR posA, DirectX::XMVECTOR posB)
{
	Vector3 normal;
	DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(posB, posA)));
	return normal;
}

std::vector<Collision> PhysicsSystem::DetectCollisions(std::vector<GameObject>& gameObjects)
{
	size_t length = gameObjects.size(); // Number of gameObjects
	std::vector<Collision> collisions; // Vector of collisions

	// If there is less than 2 game objects, there can't be any collision
	if (length < 2) return collisions;

	// At most, there can be length * (length - 1) / 2 collisions (binomial coefficient)
	collisions.reserve(length * (length - 1) / 2);

	// For each game object
	for (size_t i = 0; i < length; i++)
	{
		// Get its collider and calculate its position
		SphereCollider* colA = gameObjects[i].Get<SphereCollider>();
		DirectX::XMVECTOR posA = ColliderPosition(gameObjects[i].Get<Transform>(), colA);

		// For each game object after the current one
		for (size_t j = i + 1; j < length; j++)
		{
			// Get its collider and calculate its position
			SphereCollider* colB = gameObjects[j].Get<SphereCollider>();
			DirectX::XMVECTOR posB = ColliderPosition(gameObjects[j].Get<Transform>(), colB);

			// Add the radii and compare it to the distance between the two positions
			float radius = colA->Radius + colB->Radius;
			float sqrDistance = SqrDistanceBetween(posA, posB);
			if (radius * radius >= sqrDistance)
				// There is a collision, add it to the vector
				collisions.push_back({colA, colB, CalculateCollisionNormal(posA, posB)});
		}
	}

	return collisions;
}
}
