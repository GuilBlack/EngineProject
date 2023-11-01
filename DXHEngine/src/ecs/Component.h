#pragma once

namespace DXH
{
class GameObject;

/// <summary>
/// Component of a game object. Must be inherited by all components.
/// </summary>
struct Component
{
	virtual ~Component() = default;
	/// <summary>
	/// Automatically called when the component is added to a game object.
	/// </summary>
	virtual void OnAssign() = 0;
	/// <summary>
	/// Automatically called when the component is removed from a game object.
	/// </summary>
	virtual void OnDetach() = 0;
	/// <summary>
	/// Gets the game object that owns this component.
	/// </summary>
	GameObject* pGameObject = nullptr;
};

// Concept for classes that inherit from Component
template <typename T> concept ComponentConcept = std::is_base_of_v<Component, T>;
}
