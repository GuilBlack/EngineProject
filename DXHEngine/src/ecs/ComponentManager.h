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
private:
	ComponentManager()
	{
		// Initialize the used components map
		m_UsedComponentsMap.reserve(MAX_GO_COUNT);
	}
	~ComponentManager() = default;

public:
	/// <summary>
	/// Gets the singleton instance of this component manager.
	/// </summary>
	static ComponentManager<T>& GetInstance()
	{
		static ComponentManager<T> instance;
		return instance;
	}
	/// <summary>
	/// Gets a component from the pool and assigns it to the given game object.
	/// </summary>
	T& Assign(GameObject* target)
	{
		if (HasComponent(target))
			throw std::exception("The game object already has a component of this type.");

		// Find a component that is not in use
		static size_t index = 0;
		for (size_t i = 0; i < MAX_GO_COUNT; i++)
		{
			T& pComponent = s_Components[index];
			if (pComponent.pGameObject == nullptr)
			{
				pComponent.pGameObject = target; // Mark as used (by the target)
				pComponent.OnAssign();
				m_UsedComponentsMap.insert({target, pComponent});
				return pComponent;
			}
			index = (index + i) % MAX_GO_COUNT;
		}
		throw std::exception("No components available");
	}
	/// Detaches from the given game object the component of type T if it has one and returns it to the pool.
	/// </summary>
	void Detach(GameObject* pGameObject)
	{
		if (HasComponent(pGameObject))
		{
			T& pComponent = m_UsedComponentsMap.at(pGameObject);
			pComponent.OnDetach();
			pComponent.pGameObject = nullptr; // Mark as unused
			m_UsedComponentsMap.erase(pGameObject);
		}
	}
	/// <summary>
	/// Returns true if the given game object has a component of type T.
	/// </summary>
	bool HasComponent(const GameObject* pGameObject) const
	{
		return m_UsedComponentsMap.contains(pGameObject);
	}
	/// <summary>
	/// Returns the component of the given game object.
	/// </summary>
	T& GetComponent(const GameObject* pGameObject) const
	{
		return m_UsedComponentsMap.at(pGameObject);
	}
	/// <summary>
	/// Returns the map of all game objects that have a component of type T.
	/// </summary>
	std::unordered_map<const GameObject*, T&>& GetUsedComponentsMap() { return m_UsedComponentsMap; }

private:
	// List of all components of the given type
	T s_Components[MAX_GO_COUNT];
	// All game objects that have a component of T type
	std::unordered_map<const GameObject*, T&> m_UsedComponentsMap;
};
}
