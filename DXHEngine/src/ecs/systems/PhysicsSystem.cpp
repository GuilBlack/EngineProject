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

	std::vector<Collision> collisions = DetectCollisions(map);
	UpdateCollision(collisions, gt.DeltaTime());
}

inline DirectX::XMVECTOR PhysicsSystem::ColliderPosition(Transform* transform, SphereCollider* collider)
{
	return DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&transform->Position), DirectX::XMLoadFloat3(&collider->Center));
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

std::vector<Collision> PhysicsSystem::DetectCollisions(std::unordered_map<const GameObject*, SphereCollider*>& map)
{
	size_t length = map.size(); // Number of gameObjects
	std::vector<Collision> collisions; // Vector of collisions

	// If there is less than 2 game objects, there can't be any collision
	if (length < 2) return collisions;

	// At most, there can be length * (length - 1) / 2 collisions (binomial coefficient)
	collisions.reserve(length * (length - 1) / 2);

	// For each game object / collider pair
	for (auto pairA = map.begin(); pairA != map.end(); pairA++)
	{
		// Get its collider position
		DirectX::XMVECTOR posA = ColliderPosition(pairA->first->Get<Transform>(), pairA->second);

		// For each pair after the current one
		for (auto pairB = std::next(pairA); pairB != map.end(); pairB++)
		{
			// Get its collider position
			DirectX::XMVECTOR posB = ColliderPosition(pairA->first->Get<Transform>(), pairB->second);

			// Add the radii and compare them to the distance between the two positions
			float radius = pairA->second->Radius + pairB->second->Radius;
			float sqrDistance = SqrDistanceBetween(posA, posB);
			if (radius * radius >= sqrDistance)
				// There is a collision, add it to the vector
				collisions.push_back({pairA->second, pairB->second, CalculateCollisionNormal(posA, posB)});
		}
	}
	return collisions;
}

void PhysicsSystem::UpdateCollision(std::vector<Collision> collision, float deltaTime)
{
	using namespace DirectX;

	for (const auto& col : collision)
	{
		SphereCollider* firstSphere = col.First;
		SphereCollider* secondSphere = col.Second;

		//Vector3 collisionNormal = col.Normal;

		// Calculate relative velocity
		XMVECTOR firstSphereVelocityLoaded = XMLoadFloat3(&firstSphere->pGameObject->Get<RigidBody>()->Velocity);
		XMVECTOR secondSphereVelocityLoaded = XMLoadFloat3(&secondSphere->pGameObject->Get<RigidBody>()->Velocity);
		XMVECTOR relativeVelocity = XMVectorSubtract(firstSphereVelocityLoaded, secondSphereVelocityLoaded);
		XMVECTOR collisionNormal = XMLoadFloat3(&col.Normal);

		// Calculate impulse
		XMVECTOR impulse = XMVectorScale(XMVector3Dot(relativeVelocity, collisionNormal), -2.0f / (firstSphere->pGameObject->Get<RigidBody>()->Mass + secondSphere->pGameObject->Get<RigidBody>()->Mass));

		// Apply impulse
		XMVECTOR firstSphereVelocity = XMVectorAdd(firstSphereVelocityLoaded, XMVector3Dot(XMVectorScale(impulse, 1.0f / firstSphere->pGameObject->Get<RigidBody>()->Mass), collisionNormal));
		XMVECTOR secondSphereVelocity = XMVectorSubtract(secondSphereVelocityLoaded, XMVector3Dot(XMVectorScale(impulse, 1.0f / secondSphere->pGameObject->Get<RigidBody>()->Mass), collisionNormal));
	}
}
}
