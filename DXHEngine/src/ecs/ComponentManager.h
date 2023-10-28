#pragma once
#include "src/ecs/Component.h" // The manager needs to know what a component is

namespace DXH
{
/// <summary>
/// Static class that stores all the components of a given type.
/// </summary>
template <ComponentConcept T>
class ComponentManager
{
	// Delete the constructors, it's a static class
	ComponentManager() = delete;
	~ComponentManager() = delete;

public:
	/// <summary>
	/// Gets a component from the pool and assigns it to the given game object.
	/// </summary>
	static T* Get(GameObject* target)
	{
		static bool initialized = false;
		if (!initialized)
		{
			Init();
			initialized = true;
		}

		// Find a component that is not in use
		static size_t index = 0;
		for (size_t i = 0; i < MAX_GO_COUNT; i++)
		{
			if (s_Components[index].pGameObject == nullptr)
			{
				s_Components[index].pGameObject = target; // Mark as used (by the target)
				return &s_Components[index];
			}
			index = (index + i) % MAX_GO_COUNT;
		}
		return nullptr; // No components available
	}
	/// <summary>
	/// Releases a component back to the pool.
	/// The component is then reset to its default state.
	/// </summary>
	static void Release(T* component)
	{
		if (component == nullptr) return;
		UsedComponentsMap().erase(component->pGameObject);
		component->pGameObject = nullptr; // Mark as unused
		component->Reset();
	}
	/// <summary>
	/// Gets the map of all used components of the given type.
	/// </summary>
	inline static std::unordered_map<const GameObject*, T*>& UsedComponentsMap()
	{
		static std::unordered_map<const GameObject*, T*> map;
		return map;
	}

private:
	/// <summary>
	/// Initializes the component manager. Called automatically when the first component is requested.
	/// </summary>
	static void Init()
	{
		for (size_t i = 0; i < MAX_GO_COUNT; i++)
		{
			s_Components[i] = T();
			s_Components[i].Reset();
			s_Components[i].pGameObject = nullptr;
		}
	}

	// Array of components (size is MAX_GO_COUNT)
	inline static T* s_Components = new T[MAX_GO_COUNT];
};
}
