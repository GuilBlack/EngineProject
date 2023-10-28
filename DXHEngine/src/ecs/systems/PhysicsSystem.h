#pragma once

namespace DXH
{
struct Transform;
struct SphereCollider;
class GameObject;

/// <summary>
/// Represents a collision between two gameObjects.
/// </summary>
struct Collision
{
	SphereCollider* First = nullptr;
	SphereCollider* Second = nullptr;
	Vector3 Normal = {0,0,0}; // Normal of the collision, pointing from first to second
};

class PhysicsSystem
{    
public:
	PhysicsSystem();
	~PhysicsSystem();

	/// <summary>
	/// Calculates the position of the collider in world space
	/// </summary>
	/// <param name="transform>The transform of the game object</param>
	/// <param name="collider">The collider of the game object</param>
	inline static DirectX::XMVECTOR ColliderPosition(Transform* transform, SphereCollider* collider);
	/// <summary>
	/// Calculates the squared distance between two positions
	/// </summary>
	inline static float SqrDistanceBetween(DirectX::XMVECTOR posA, DirectX::XMVECTOR posB);
	/// <summary>
	/// Calculates the normal of the collision between two positions
	/// </summary>
	inline static Vector3 CalculateCollisionNormal(DirectX::XMVECTOR posA, DirectX::XMVECTOR posB);
	/// <summary>
	/// Detects any collisions between all passed gameObjects
	/// </summary>
	/// <param name="gameObjects">The gameObjects to check for collisions. All gameObjects must have a sphere collider</param>
	static std::vector<Collision> DetectCollisions(std::vector<DXH::GameObject>& gameObjects);
};
}
