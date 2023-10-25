#pragma once
#include "src/core/Timer.h"
#include "src/ecs/components/Physics.h"

namespace DXH
{

class PhysicsSystem
{
	public:
	PhysicsSystem() = default;
	~PhysicsSystem() = default;
	
	
	bool IsColliding(const SphereCollider* first, const SphereCollider* second) const;

	void AddRigidBody(RigidBody* rigidBody);
	void RemoveRigidBody(RigidBody* rigidBody);

	void AddSphereCollider(SphereCollider* sphereCollider);
	void RemoveSphereCollider(SphereCollider* sphereCollider);

	void ResponseToCollide(const SphereCollider* first, const SphereCollider* second);
	void Update(const Timer& gt);	

private:
	std::vector<RigidBody*> m_RigidBodies;
	std::vector<SphereCollider*> m_SphereColliders;
};
}
