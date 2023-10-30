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
	/// Automatically called when the component is retrieved from the pool (added to a game object).
	/// </summary>
	virtual void OnGet() = 0;
	/// <summary>
	/// Automatically called when the component is released back to the pool (removed from a game object).
	/// </summary>
	virtual void OnRelease() = 0;
	/// <summary>
	/// Gets the game object that owns this component.
	/// </summary>
	GameObject* pGameObject = nullptr;
};

// Concept for classes that inherit from Component
template <typename T> concept ComponentConcept = std::is_base_of_v<Component, T>;
}
