#pragma once
#include "src/ecs/System.h" // For the System base class
#include "src/DXHMaths.h" // For the Vector3 struct

namespace DXH
{
// Forward declarations, avoid including the headers
struct Transform;
struct SphereCollider;
class GameObject;

/// <summary>
/// Represents a collision between two gameObjects.
/// </summary>
struct Collision
{
	SphereCollider* First;
	SphereCollider* Second;
	Vector3 Normal = Vector3::Zero; // Normal of the collision, pointing from first to second
	float sqrDiff = 0.f; //Difference between the radii of the two colliders and the distance between them
};

struct Cell
{
	Vector3 Min;
	Vector3 Max;
	std::vector<SphereCollider*> Colliders;
};

class PhysicsSystem : public System
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void Update(const Timer& gt) override;

private:

	std::vector<Cell> m_Cells;
	std::vector<Collision> m_Collisions;

	int m_NumberOfCollisions = 0;
	int m_NumberOfCells = 0;

	/// <summary>
	/// Calculates the position of the collider in world space
	/// </summary>
	/// <param name="transform>The transform of the game object</param>
	/// <param name="collider">The collider of the game object</param>
	inline static DirectX::XMVECTOR ColliderPosition(Transform& transform, SphereCollider& collider);
	/// <summary>
	/// Calculates the squared distance between two positions
	/// </summary>
	inline static float SqrDistanceBetween(DirectX::FXMVECTOR& posA, DirectX::FXMVECTOR& posB);
	/// <summary>
	/// Calculates the normal of the collision between two positions
	/// </summary>
	inline static Vector3 CalculateCollisionNormal(DirectX::FXMVECTOR& posA, DirectX::FXMVECTOR& posB);

	void SortColliders(std::unordered_map<const GameObject*, SphereCollider&>& gameObjects, float cellSize);
	/// <summary>
	/// Detects any collisions between all gameObjects in the passed cell.
	/// </summary>
	/// <param name="gameObjects">The gameObjects to check for collisions. All gameObjects must have a sphere collider</param>
	void DetectCollisions(Cell& cell);

	void ApplyCollisions(float deltaTime);

	void UpdateRigidBodies(const Timer& gt);
};
}
