#include "PhysicsSystem.h"
namespace DXH
{
PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

std::vector<Collision> PhysicsSystem::DetectCollisions(std::vector<Entity>& entities)
{
	size_t length = entities.size(); // Number of entities
	std::vector<Collision> collisions; // Vector of collisions

	// If there is less than 2 entities, there can't be any collision
	if (length < 2) return collisions;

	// At most, there can be length * (length - 1) / 2 collisions (binomial coefficient)
	collisions.reserve(length * (length - 1) / 2);

	// For each entity
	for (size_t i = 0; i < length; i++)
	{
		// Get its collider and calculate its position
		SphereCollider* colA = entities[i].GetComponent<SphereCollider>();
		DirectX::XMVECTOR posA = ColliderPosition(entities[i].GetTransform(), *colA);

		// For each entity after the current one
		for (size_t j = i + 1; j < length; j++)
		{
			// Get its collider and calculate its position
			SphereCollider* colB = entities[j].GetComponent<SphereCollider>();
			DirectX::XMVECTOR posB = ColliderPosition(entities[j].GetTransform(), *colB);

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
void PhysicsSystem::UpdateCollision(std::vector<Collision> collision, float deltaTime)
{
	using namespace DirectX; 

	for (const auto& col : collision)
	{
		SphereCollider* firstSphere = col.First;
		SphereCollider* secondSphere = col.Second;

		//Vector3 collisionNormal = col.Normal;

		// Calculate relative velocity
		XMVECTOR firstSphereVelocityLoaded = XMLoadFloat3(&firstSphere->GetEntity()->GetComponent<RigidBody>()->Velociy);
		XMVECTOR secondSphereVelocityLoaded = XMLoadFloat3(&secondSphere->GetEntity()->GetComponent<RigidBody>()->Velociy);
		XMVECTOR relativeVelocity = XMVectorSubtract(firstSphereVelocityLoaded, secondSphereVelocityLoaded);
		XMVECTOR collisionNormal = XMLoadFloat3(&col.Normal);

		// Calculate impulse
		XMVECTOR impulse = XMVectorScale(XMVector3Dot(relativeVelocity, collisionNormal), -2.0f/(firstSphere->GetEntity()->GetComponent<RigidBody>()->Mass + secondSphere->GetEntity()->GetComponent<RigidBody>()->Mass));

		// Apply impulse
		XMVECTOR firstSphereVelocity = XMVectorAdd(firstSphereVelocityLoaded, XMVector3Dot(XMVectorScale(impulse, 1.0f/firstSphere->GetEntity()->GetComponent<RigidBody>()->Mass), collisionNormal));
		XMVECTOR secondSphereVelocity = XMVectorSubtract(secondSphereVelocityLoaded, XMVector3Dot(XMVectorScale(impulse, 1.0f / secondSphere->GetEntity()->GetComponent<RigidBody>()->Mass), collisionNormal));
	}
}
}
