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

	Timer t;
	t.Tick();
	SortColliders(map, 10.0f);
	t.Tick();
	VS_DB_OUT_A("SortColliders: " << t.DeltaTime() * 1000<< "ms\n");

	
	for (int i = 0; i < m_NumberOfCells ; i++)
	{

		t.Tick();
		DetectCollisions(m_Cells[i]);
		t.Tick();
		VS_DB_OUT_A("DetectCollisions" << t.DeltaTime() * 1000 << "ms\n");
		t.Tick();
		ApplyCollisions(gt.DeltaTime());
		t.Tick();
		VS_DB_OUT_A("Apply: " << t.DeltaTime() * 1000 << "ms\n");
		t.Tick();
		m_Cells[i].Colliders.clear();
		t.Tick();
		VS_DB_OUT_A("Clear: " << t.DeltaTime() * 1000 << "ms\n");
	}
	

	t.Tick();
	UpdateRigidBodies(gt);
	t.Tick();
	VS_DB_OUT_A("UpdateRigidBodies: " << t.DeltaTime() * 1000 << "ms\n");
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
			float diff = radius * radius - sqrDistance;
			if (diff >= 0)
			{
				// There is a collision, add it to the vector
				if(m_Collisions.size() <= m_NumberOfCollisions)
					m_Collisions.emplace_back();

				m_Collisions[m_NumberOfCollisions].First = collA;
				m_Collisions[m_NumberOfCollisions].Second = collB;
				m_Collisions[m_NumberOfCollisions].Normal = CalculateCollisionNormal(posA, posB);
				m_Collisions[m_NumberOfCollisions].sqrDiff = diff;
				m_NumberOfCollisions++;
			}
		}
	}
}

void PhysicsSystem::ApplyCollisions(float deltaTime)
{
	using namespace DirectX;

	for (int i = 0; i < m_NumberOfCollisions; i++)
	{
		if(m_Collisions[i].sqrDiff == 0) continue; //They're exaclty on top of each other, no need to apply impulse

		SphereCollider* firstSphere = m_Collisions[i].First;
		SphereCollider* secondSphere = m_Collisions[i].Second;

		firstSphere->pGameObject->Get<RigidBody>()->Velocity.Store(firstSphere->pGameObject->Get<RigidBody>()->Velocity.Load() + XMVectorScale(m_Collisions[i].Normal.Load(), m_Collisions[i].sqrDiff * -1));
		secondSphere->pGameObject->Get<RigidBody>()->Velocity.Store(secondSphere->pGameObject->Get<RigidBody>()->Velocity.Load() + XMVectorScale(m_Collisions[i].Normal.Load(), m_Collisions[i].sqrDiff));

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
