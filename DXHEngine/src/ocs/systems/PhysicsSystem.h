#pragma once
#include "src/core/Timer.h"

namespace DXH
{
class Entity;
class Component;

struct RigidBody
{
	Vector3 m_Position; //TODO : Change to Transform
	Vector3 m_Velocity;
	Vector3 m_Force;
	float m_Mass;
	const Vector3 m_Gravity = Vector3(0, -9.81f, 0);

	Collider* m_Collider;
	Transform* m_Transform;
};

struct CollisionPoints
{
	Vector3 m_Point1; //furthest point A to B
	Vector3 m_Point2; //furthest point B to A
	Vector3 m_Normal; // B - A normalized 
	float m_Depth; // Length of B - A 
	bool m_IsColliding;
};

struct Transform
{
	Vector3 m_Position;
	Quaternion m_Rotation;
	Vector3 m_Scale;
};

struct Collider 
{
	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const Collider* collider,
		const Transform* colliderTransform) const = 0;

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const SphereCollider* sphere,
		const Transform* sphereTransform) const = 0;
};

struct SphereCollider : Collider 
{
	Vector3 center;
	float radius;

	CollisionPoints TestCollision(
		const Transform* transform,
		const Collider* collider,
		const Transform* colliderTransform) const override;

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const SphereCollider* sphere,
		const Transform* sphereTransform) const override;
};

struct Collision
{
	RigidBody* m_RigidBody1;
	RigidBody* m_RigidBody2;
	CollisionPoints m_CollisionPoints;
};

class Solver
{
public:
	virtual void Solve(std::vector<Collision>& collisions, float dt) = 0; 
};

class PhysicsSystem
{
	public:
	PhysicsSystem() = default;
	~PhysicsSystem() = default;
	
	
	void AddRigidBody(RigidBody* rigidBody);
	void RemoveRigidBody(RigidBody* rigidBody);

	void AddSolver(Solver* solver);
	void RemoveSolver(Solver* solver);

	void Update(const Timer& gt);
	void ResolveCollision(const Timer& gt);

private:
	std::vector<RigidBody*> m_RigidBody;
	std::vector<Solver*> m_Solvers;
};
}
