#pragma once
#include <map>
#include <functional>

namespace DXH
{
class Entity; // Forward declaration of Entity
class Component; // Forward declaration of Component

// Concept for component types
template <typename T> concept
ComponentChild = std::is_base_of_v<Component, T>;
class Component // Component must inherit from this
{
public:
	/// <summary>
	/// Create a new component attached to an entity.
	/// </summary>
	/// <param name="e">Entity to attach to.</param>
	Component(Entity* e) : m_Entity(e) {}
	virtual ~Component() { m_Entity = nullptr; }

	/// <summary>
	/// Id of the component type.
	/// </summary>
	typedef uint32_t Id;
	/// <summary>
	/// Gets the id of a component type.
	/// </summary>
	/// <typeparam name="C">Component type.</typeparam>
	template <ComponentChild C> static Id GetId()
	{
		static Id id = GetNewId();
		return id;
	}

private:
	Entity* m_Entity; // Id of the entity this component is attached to

private: // "Manager" stuff
	/// <summary>
	/// Gets a new id for a component type. (incremental)
	/// </summary>
	static inline Id GetNewId()
	{
		static Id id = 0;
		return id++;
	}
};
}
