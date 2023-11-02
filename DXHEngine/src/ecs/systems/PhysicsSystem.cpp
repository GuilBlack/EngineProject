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


	SortColliders(map, 10.0f);

	for (auto& cell : m_Cells)
	{
		DetectCollisions(cell);
		ApplyCollisions(gt.DeltaTime());
		cell.Colliders.clear();
	}

	UpdateRigidBodies(gt);
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


void PhysicsSystem::SortColliders(std::unordered_map<const GameObject*, SphereCollider*>& gameObjects, float cellSize)
{
	m_NumberOfCells = 0;
	for (auto& [gameObject, collider] : gameObjects)
	{
		Vector3 center = ColliderPosition(gameObject->Get<Transform>(), collider);
		bool Found = false;
		for (auto& cell : m_Cells)
		{
			if (center.x >= cell.Min.x && center.x <= cell.Max.x &&
				center.y >= cell.Min.y && center.y <= cell.Max.y &&
				center.z >= cell.Min.z && center.z <= cell.Max.z)
			{
				cell.Colliders.push_back(collider); 
				Found = true;
				m_NumberOfCollisions++;
				break;
			}
		}
		// If not found create a new cell and add it to the vector of cells
		if (!Found)
		{
			if (m_Cells.size() <= m_NumberOfCells)
				m_Cells.emplace_back(); 

			m_Cells[m_NumberOfCells].Min = Vector3(floorf(center.x / cellSize) * cellSize, floorf(center.y / cellSize) * cellSize, floorf(center.z / cellSize) * cellSize);
			m_Cells[m_NumberOfCells].Max = Vector3(m_Cells[m_NumberOfCells].Min.x + cellSize, m_Cells[m_NumberOfCells].Min.y + cellSize, m_Cells[m_NumberOfCells].Min.z + cellSize);
			m_Cells[m_NumberOfCells].Colliders.push_back(collider);
			m_NumberOfCells++;
			
		}
	}
}


void PhysicsSystem::DetectCollisions(Cell& cell)
{
	size_t length = cell.Colliders.size(); // Number of gameObjects
	m_NumberOfCollisions = 0;

	// If there is less than 2 game objects, there can't be any collision
	if (length < 2) return;

	// For each game object / collider pair
	for (size_t i = 0; i < length - 1; i++)
	{
		// Get its collider position
		SphereCollider* collA = cell.Colliders[i];
		DirectX::XMVECTOR posA = ColliderPosition(collA->pGameObject->Get<Transform>(), collA);

		// For each pair after the current one
		for (size_t j = i + 1; j < length; j++)
		{
			// Get its collider position
			SphereCollider* collB = cell.Colliders[j];
			DirectX::XMVECTOR posB = ColliderPosition(collB->pGameObject->Get<Transform>(), collB);

			// Add the radii and compare them to the distance between the two positions
			float radius = collA->Radius + collB->Radius;
			float sqrDistance = SqrDistanceBetween(posA, posB);
			if (radius * radius >= sqrDistance)
			{
				// There is a collision, add it to the vector
				if(m_Collisions.size() <= m_NumberOfCollisions)
					m_Collisions.emplace_back();

				m_Collisions[m_NumberOfCollisions].First = collA;
				m_Collisions[m_NumberOfCollisions].Second = collB;
				m_Collisions[m_NumberOfCollisions].Normal = CalculateCollisionNormal(posA, posB);
				m_NumberOfCollisions++;
				VS_DB_OUT_A("TEST \n");
			}
		}
	}
}

void PhysicsSystem::ApplyCollisions(float deltaTime)
{
	using namespace DirectX;

	for (int i = 0; i < m_NumberOfCollisions; i++)
	{
		SphereCollider* firstSphere = m_Collisions[i].First;
		SphereCollider* secondSphere = m_Collisions[i].Second;

		//Vector3 collisionNormal = col.Normal;

		// Calculate relative velocity
		XMVECTOR firstSphereVelocityLoaded = firstSphere->pGameObject->Get<RigidBody>()->Velocity.Load();
		XMVECTOR secondSphereVelocityLoaded = secondSphere->pGameObject->Get<RigidBody>()->Velocity.Load();
		XMVECTOR relativeVelocity = XMVectorSubtract(firstSphereVelocityLoaded, secondSphereVelocityLoaded);
		XMVECTOR collisionNormal = m_Collisions[i].Normal.Load();

		// Calculate impulse
		XMVECTOR impulse = XMVectorScale(XMVector3Dot(relativeVelocity, collisionNormal), -2.0f / (firstSphere->pGameObject->Get<RigidBody>()->Mass + secondSphere->pGameObject->Get<RigidBody>()->Mass));

		// Apply impulse
		XMVECTOR firstSphereVelocity = XMVectorAdd(firstSphereVelocityLoaded, XMVector3Dot(XMVectorScale(impulse, 1.0f / firstSphere->pGameObject->Get<RigidBody>()->Mass), collisionNormal));
	
		XMVECTOR secondSphereVelocity = XMVectorSubtract(secondSphereVelocityLoaded, XMVector3Dot(XMVectorScale(impulse, 1.0f / secondSphere->pGameObject->Get<RigidBody>()->Mass), collisionNormal));

		firstSphere->pGameObject->Get<RigidBody>()->Velocity.Store(firstSphereVelocity);
		secondSphere->pGameObject->Get<RigidBody>()->Velocity.Store(secondSphereVelocity);
	}
}
void PhysicsSystem::UpdateRigidBodies(const Timer& gt)
{
	using namespace DirectX; 
	auto& rigidBodies = ComponentManager<RigidBody>::GetInstance().GetUsedComponentsMap();
	for (auto& [gameObject, rigidBody] : rigidBodies)
	{
		XMVECTOR velocity = XMVectorScale(rigidBody->Force.Load(), 1.f/rigidBody->Mass);
		rigidBody->Velocity = XMVectorAdd(rigidBody->Velocity.Load(), XMVectorScale(velocity, gt.DeltaTime()));
		gameObject->Get<Transform>()->Position = XMVectorAdd(gameObject->Get<Transform>()->Position.Load(), XMVectorScale(rigidBody->Velocity.Load(), gt.DeltaTime()));
	}
}
}
