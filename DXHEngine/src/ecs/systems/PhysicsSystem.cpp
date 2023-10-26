#include "PhysicsSystem.h"
namespace DXH
{
PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

std::vector<Collision> PhysicsSystem::ComputeCollisions(std::vector<DXH::Entity>& entities)
{
	using namespace DirectX;
	size_t length = entities.size(); // Number of entities
	int count = 0; // Number of comparisons

	std::vector<Collision> collisions; // Vector of collisions
	collisions.reserve(length * (length - 1) / 2); // Binomial coefficient of n over 2

	size_t i = 0, j; // Loop variables (we need to use them outside the loop)
	for (; i < length - 3; i += 4)
	{
		// Get 4 "first" colliders
		auto colA1 = entities[i].GetComponent<DXH::SphereCollider>();
		auto colA2 = entities[i + 1].GetComponent<DXH::SphereCollider>();
		auto colA3 = entities[i + 2].GetComponent<DXH::SphereCollider>();
		auto colA4 = entities[i + 3].GetComponent<DXH::SphereCollider>();

		// Get 4 collider positions
		XMVECTOR posA1 = ColliderPosition(entities[i].GetTransform(), *colA1);
		XMVECTOR posA2 = ColliderPosition(entities[i + 1].GetTransform(), *colA2);
		XMVECTOR posA3 = ColliderPosition(entities[i + 2].GetTransform(), *colA3);
		XMVECTOR posA4 = ColliderPosition(entities[i + 3].GetTransform(), *colA4);

		for (j = i + 1; j < length - 3; j++)
		{
			// Get 4 "second" colliders
			auto colB1 = entities[j].GetComponent<DXH::SphereCollider>();
			auto colB2 = entities[j + 1].GetComponent<DXH::SphereCollider>();
			auto colB3 = entities[j + 2].GetComponent<DXH::SphereCollider>();
			auto colB4 = entities[j + 3].GetComponent<DXH::SphereCollider>();

			// Get 4 collider positions
			XMVECTOR posB1 = ColliderPosition(entities[j].GetTransform(), *colB1);
			XMVECTOR posB2 = ColliderPosition(entities[j + 1].GetTransform(), *colB2);
			XMVECTOR posB3 = ColliderPosition(entities[j + 2].GetTransform(), *colB3);
			XMVECTOR posB4 = ColliderPosition(entities[j + 3].GetTransform(), *colB4);

			// Get 4 radii and 4 squared lengths between colliders
			XMVECTOR radius = XMVectorAdd(
				XMVectorSet(colA1->Radius, colA2->Radius, colA3->Radius, colA4->Radius),
				XMVectorSet(colB1->Radius, colB2->Radius, colB3->Radius, colB4->Radius));
			XMVECTOR sqrRad = XMVectorMultiply(radius, radius);
			XMVECTOR sqrLen = XMVectorSet(
				SqrDistanceBetween(posA1, posB1),
				SqrDistanceBetween(posA2, posB2),
				SqrDistanceBetween(posA3, posB3),
				SqrDistanceBetween(posA4, posB4));

			// Compare squared lengths with squared radii
			uint32_t CR;
			XMVectorGreaterOrEqualR(&CR, sqrRad, sqrLen);
			if (CR & 0b0001)
				collisions.push_back({colA1, colB1, { 0, 0, 0}});
			if (CR & 0b0010)
				collisions.push_back({colA2, colB2, { 0, 0, 0}});
			if (CR & 0b0100)
				collisions.push_back({colA3, colB3, { 0, 0, 0}});
			if (CR & 0b1000)
				collisions.push_back({colA4, colB4, { 0, 0, 0}});

			count += 4;
		}
	}

	// Original check / Less than 4 colliders left
	for (; i < length; i++)
	{
		auto colA = entities[i].GetComponent<DXH::SphereCollider>();
		XMVECTOR posA = ColliderPosition(entities[i].GetTransform(), *colA);

		for (j = i + 1; j < length; j++)
		{
			auto colB = entities[j].GetComponent<DXH::SphereCollider>();
			float radius = colA->Radius + colB->Radius;
			float sqrLen = SqrDistanceBetween(posA, ColliderPosition(entities[j].GetTransform(), *colB));
			if (radius * radius >= sqrLen)
				collisions.push_back({colA, colB, { 0, 0, 0}});

			count++;
		}
	}

	// TODO : Calculate normals
	VS_DB_OUT_A("Comparisons : " << count << " / " << length * (length - 1) / 2 << std::endl);
	return collisions;
}
}
