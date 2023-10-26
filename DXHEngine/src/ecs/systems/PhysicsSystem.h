#pragma once
#include "src/core/Timer.h"
#include "src/ecs/components/Physics.h"

namespace DXH
{

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	/// <summary>
	/// Calculates the position of the collider in world space
	/// </summary>
	/// <param name="transform>The transform of the entity</param>
	/// <param name="collider">The collider of the entity</param>
	inline static DirectX::XMVECTOR ColliderPosition(const DXH::Transform& transform, const DXH::SphereCollider& collider)
	{
		return DirectX::XMVectorAdd(XMLoadFloat3(&transform.position), XMLoadFloat3(&collider.Center));
	}
	/// <summary>
	/// Calculates the squared distance between two positions
	/// </summary>
	inline static float SqrDistanceBetween(DirectX::XMVECTOR posA, DirectX::XMVECTOR posB)
	{
		return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(posA, posB)));
	}
	/// <summary>
	/// Computes the collision between all passed entities
	/// </summary>
	/// <param name="entities">The entities to check for collisions. All entities must have a sphere collider</param>
	static std::vector<Collision> ComputeCollisions(std::vector<DXH::Entity>& entities);

private:
	std::vector<SphereCollider*> m_SphereColliders;
};
}
