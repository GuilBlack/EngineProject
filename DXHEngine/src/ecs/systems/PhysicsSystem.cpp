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
}
