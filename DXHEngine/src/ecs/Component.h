#pragma once

namespace DXH
{
class GameObject;

/// <summary>
/// Component of a game object. Must be inherited by all components.
/// </summary>
struct Component
{
	/// <summary>
	/// Resets the component to its default state.
	/// Automatically called when the component is created and released.
	/// </summary>
	virtual void Reset() = 0;
	/// <summary>
	/// Gets the game object that owns this component.
	/// </summary>
	GameObject* pGameObject = nullptr;
};

// Concept for classes that inherit from Component
template <typename T> concept ComponentConcept = std::is_base_of_v<Component, T>;
}
