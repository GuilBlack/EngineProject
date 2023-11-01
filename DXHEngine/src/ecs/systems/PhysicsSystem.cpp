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
	auto& map = ComponentManager<SphereCollider>::GetInstance().GetUsedComponentsMap();

	std::vector<Collision> collisions = DetectCollisions(map);
	UpdateCollision(collisions, gt.DeltaTime());
}

inline DirectX::XMVECTOR PhysicsSystem::ColliderPosition(Transform* transform, SphereCollider* collider)
{
	return DirectX::XMVectorAdd(transform->Position.Load(), collider->Center.Load());
}

inline float PhysicsSystem::SqrDistanceBetween(DirectX::FXMVECTOR posA, DirectX::FXMVECTOR posB)
{
	return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(posA, posB)));
}

inline Vector3 PhysicsSystem::CalculateCollisionNormal(DirectX::FXMVECTOR posA, DirectX::FXMVECTOR posB)
{
	return Vector3(DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(posB, posA)));
}

std::vector<Collision> PhysicsSystem::DetectCollisions(std::vector<Cell>)
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
		XMVECTOR firstSphereVelocityLoaded = firstSphere->pGameObject->Get<RigidBody>()->Velocity.Load();
		XMVECTOR secondSphereVelocityLoaded = secondSphere->pGameObject->Get<RigidBody>()->Velocity.Load();
		XMVECTOR relativeVelocity = XMVectorSubtract(firstSphereVelocityLoaded, secondSphereVelocityLoaded);
		XMVECTOR collisionNormal = col.Normal.Load();

		// Calculate impulse
		XMVECTOR impulse = XMVectorScale(XMVector3Dot(relativeVelocity, collisionNormal), -2.0f / (firstSphere->pGameObject->Get<RigidBody>()->Mass + secondSphere->pGameObject->Get<RigidBody>()->Mass));

		// Apply impulse
		XMVECTOR firstSphereVelocity = XMVectorAdd(firstSphereVelocityLoaded, XMVector3Dot(XMVectorScale(impulse, 1.0f / firstSphere->pGameObject->Get<RigidBody>()->Mass), collisionNormal));
		XMVECTOR secondSphereVelocity = XMVectorSubtract(secondSphereVelocityLoaded, XMVector3Dot(XMVectorScale(impulse, 1.0f / secondSphere->pGameObject->Get<RigidBody>()->Mass), collisionNormal));
	}
}

std::vector<Cell> PhysicsSystem::SortColliders(std::unordered_map<const GameObject*, SphereCollider*>& gameObjects, float cellSize)
{
	std::vector<Cell> cells; 
	for (auto[gameObject, collider] : gameObjects)
	{
		Vector3 center = ColliderPosition(gameObject->Get<Transform>(), collider);
		bool Found = false;
		for (auto& cell : cells)
		{
			if (center.x >= cell.Min.x && center.x <= cell.Max.x &&
				center.y >= cell.Min.y && center.y <= cell.Max.y &&
				center.z >= cell.Min.z && center.z <= cell.Max.z)
			{
				cell.Colliders.push_back(collider);
				Found = true;
				break;
			}
		}
		// If not found create a new cell and add it to the vector of cells
		if (!Found)
		{
			Cell cell;
			cell.Min = Vector3(floorf(center.x / cellSize) * cellSize, floorf(center.y / cellSize) * cellSize, floorf(center.z / cellSize) * cellSize);
			cell.Max = Vector3(cell.Min.x + cellSize, cell.Min.y + cellSize, cell.Min.z + cellSize);
			cell.Colliders.push_back(collider);
			cells.push_back(cell);
		}
	}
	return std::vector<Cell>();
}
}
