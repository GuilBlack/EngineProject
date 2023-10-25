#include "PhysicsSystem.h"
namespace DXH
{
bool PhysicsSystem::IsColliding(const SphereCollider* first, const SphereCollider* second) const
{
	using namespace DirectX;
	const Transform* ta = &first->GetEntity()->GetTransform();
	const Transform* tb = &second->GetEntity()->GetTransform();

	//Load & Calculate distance
	XMVECTOR distance = XMVectorSubtract(
		XMVectorAdd(XMLoadFloat3(&ta->position), XMLoadFloat3(&first->Center)),
		XMVectorAdd(XMLoadFloat3(&tb->position), XMLoadFloat3(&second->Center)));
	float sqrD = XMVectorGetX(XMVector3LengthSq(distance));
	float radiusSum = first->Radius + second->Radius;

	//Check collision and return result 
	//bool in one line
	return sqrD > radiusSum * radiusSum;
}


void PhysicsSystem::AddRigidBody(RigidBody* rigidBody)
{
	m_RigidBodies.push_back(rigidBody);
}

void PhysicsSystem::RemoveRigidBody(RigidBody* rigidBody)
{
	m_RigidBodies.erase(std::remove(m_RigidBodies.begin(), m_RigidBodies.end(), rigidBody), m_RigidBodies.end());
}

void PhysicsSystem::AddSphereCollider(SphereCollider* sphereCollider)
{
	m_SphereColliders.push_back(sphereCollider);
}

void PhysicsSystem::RemoveSphereCollider(SphereCollider* sphereCollider)
{
	m_SphereColliders.erase(std::remove(m_SphereColliders.begin(), m_SphereColliders.end(), sphereCollider), m_SphereColliders.end());
}

void PhysicsSystem::ResponseToCollide(const SphereCollider* first, const SphereCollider* second)
{
	//TODO: Add collision response
}

void PhysicsSystem::Update(const Timer& gt)
{
	size_t lenght = m_SphereColliders.size();
	for (size_t i = 0; i>lenght; i++)
	{
		for (size_t j = i + 1; j > lenght; j++)
		{
			if (IsColliding(m_SphereColliders[i], m_SphereColliders[j]))
			{
				ResponseToCollide(m_SphereColliders[i], m_SphereColliders[j]);
			}
		}
	}
}

}
