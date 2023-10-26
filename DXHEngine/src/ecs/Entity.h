#pragma once
#include "Component.h"

namespace DXH
{
struct Transform
{
	Vector3 position = {0, 0, 0};
	Quaternion rotation = {0, 0, 0, 1};
	Vector3 scale = {1, 1, 1};

	/// <summary>
	/// Gets the matrix describing the transformation from world space to model space.
	/// </summary>
	inline Matrix GetModelMatrix() const { return DirectX::XMMatrixAffineTransformation(XMLoadFloat3(&scale), DirectX::XMVectorZero(), XMLoadFloat4(&rotation), XMLoadFloat3(&position)); }
};

class Entity
{
public:
	typedef uint32_t Id; // Represents the id of an entity.
	Entity(Id* componentIds, uint32_t componentCount); // Create an entity with the given components.
	Entity() : Entity(nullptr, 0) {} // Create an entity with no components.

	/// <summary>
	/// Gets the id of the entity.
	/// </summary>
	inline Id GetId() const { return m_id; }
	/// <summary>
	/// Returns transform of the entity.
	/// </summary>
	inline const Transform& GetTransform() const { return m_Transform; }
	/// <summary>
	/// Adds a component to the entity.
	/// </summary>
	/// <typeparam name="C">Type of the component to add.</typeparam>
	template <ComponentChild C>
	inline void AddComponent() { m_components[Component::GetId<C>()] = new C(this); }
	/// <summary>
	/// Removes a component from the entity.
	/// </summary>
	/// <typeparam name="C">Type of the component to remove.</typeparam>
	template <ComponentChild C>
	inline void RemoveComponent() { m_components.erase(Component::GetId<C>()); }
	/// <summary>
	/// Gets a component from the entity.
	/// </summary>
	/// <typeparam name="C">Type of the component to get.</typeparam>
	template <ComponentChild C>
	inline C* GetComponent() const { return static_cast<C*>(m_components.at(Component::GetId<C>())); }
	/// <summary>
	/// Checks if the entity has a component.
	/// </summary>
	/// <typeparam name="C">Type of the component to check.</typeparam>
	template <ComponentChild C>
	inline bool HasComponent() const { return m_components.find(Component::GetId<C>()) != m_components.end(); }
	/// <summary>
	/// Tries to get a component from the entity.
	/// </summary>
	/// <typeparam name="C">Type of the component to get.</typeparam>
	/// <param name="out">Pointer to the component, if found.</param>
	/// <returns>True if the component was found, false otherwise.</returns>
	template <ComponentChild C>
	bool TryGetComponent(C* out) const;

private:
	Id m_id; // Unique id of the entity.
	Transform m_Transform; // Transform of the entity.
	std::unordered_map<Id, Component*> m_components; // Components of the entity.

	inline static Id GetNewId() { static Id id = 0; return id++; }
};
}
