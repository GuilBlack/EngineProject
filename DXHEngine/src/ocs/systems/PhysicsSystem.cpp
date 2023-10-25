#include "PhysicsSystem.h"

void DXH::PhysicsSystem::AddRigidBody(RigidBody* rigidBody)
{
	m_RigidBody.push_back(rigidBody);
}

void DXH::PhysicsSystem::RemoveRigidBody(RigidBody* rigidBody)
{
	if (!rigidBody) return;
	auto itr = std::find(m_RigidBody.begin(), m_RigidBody.end(), rigidBody);
	if(itr == m_RigidBody.end()) return;
	m_RigidBody.erase(itr);
}

void DXH::PhysicsSystem::AddSolver(Solver* solver)
{
	if (solver == nullptr)
	{
		VS_DB_OUT_W(L"Solver is nullptr!\n");
		return;
	}
	m_Solvers.push_back(solver);
}

void DXH::PhysicsSystem::RemoveSolver(Solver* solver)
{
	if (solver == nullptr)
	{
		VS_DB_OUT_W(L"Solver is nullptr!\n");
		return;
	}
	auto itr = std::find(m_Solvers.begin(), m_Solvers.end(), solver);
	if (itr == m_Solvers.end())
	{
		VS_DB_OUT_W(L"Solver is not in the list!\n");
		return;
	}
	m_Solvers.erase(itr);
}

void DXH::PhysicsSystem::Update(const Timer& gt)
{
	float deltaTime = gt.DeltaTime();
	ResolveCollision(gt);
	for (RigidBody* rigidBody : m_RigidBody)
	{
		// load resssources
		DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&rigidBody->m_Position);
		DirectX::XMVECTOR velocity = DirectX::XMLoadFloat3(&rigidBody->m_Velocity);
		DirectX::XMVECTOR force = DirectX::XMLoadFloat3(&rigidBody->m_Force);
		DirectX::XMVECTOR gravity = DirectX::XMLoadFloat3(&rigidBody->m_Gravity);

		// make calculations
		
		force = DirectX::XMVectorAdd(force, DirectX::XMVectorScale(gravity, rigidBody->m_Mass));
		velocity = DirectX::XMVectorAdd(velocity, DirectX::XMVectorScale(force, 1.0f / rigidBody->m_Mass * deltaTime));
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(velocity, deltaTime));
		force = DirectX::XMVectorZero();

		// store results
		DirectX::XMStoreFloat3 (&rigidBody->m_Position, position);
		DirectX::XMStoreFloat3(&rigidBody->m_Velocity, velocity);
		DirectX::XMStoreFloat3(&rigidBody->m_Force, force);

	}
}

void DXH::PhysicsSystem::ResolveCollision(const Timer& gt)
{
	std::vector<Collision> collisions;
	for (RigidBody* a : m_RigidBody)
	{
		for (RigidBody* b : m_RigidBody)
		{
			if(a == b) break; //unique pairs

			if(!a->m_Collider || !b->m_Collider) continue; // both have colliders

			CollisionPoints points = a->m_Collider->TestCollision(a->m_Transform, b->m_Collider, b->m_Transform);

			if (points.m_IsColliding)
			{
				collisions.emplace_back(a, b, points);
			}
		}
	}
	//Solve collisions
	for (Solver* solver : m_Solvers)
	{
		solver->Solve(collisions, gt.DeltaTime());
	}
}

DXH::CollisionPoints DXH::SphereCollider::TestCollision(const Transform* transform, const Collider* collider, const Transform* colliderTransform) const
{
	return collider->TestCollision(colliderTransform, this, transform);
}

DXH::CollisionPoints DXH::SphereCollider::TestCollision(const Transform* transform, const SphereCollider* sphere, const Transform* sphereTransform) const
{
	const SphereCollider* a = this;
	const Transform* ta = transform;
	const SphereCollider* b = sphere;
	const Transform* tb = sphereTransform;
	return CollisionPoints();
}
