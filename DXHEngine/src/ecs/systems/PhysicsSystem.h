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
	/// Calculates the normal of the collision between two positions
	/// </summary>
	inline static Vector3 CalculateCollisionNormal(DirectX::XMVECTOR posA, DirectX::XMVECTOR posB)
	{
		Vector3 normal;
		DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(posB, posA)));
		return normal;
	}
	/// <summary>
	/// Detects any collisions between all passed entities
	/// </summary>
	/// <param name="entities">The entities to check for collisions. All entities must have a sphere collider</param>
	static std::vector<Collision> DetectCollisions(std::vector<DXH::Entity>& entities);

	void UpdateCollision(std::vector<Collision> collision, float deltaTime);
};
}
