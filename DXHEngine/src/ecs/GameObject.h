#pragma once
#include "ComponentManager.h" // Mandatory include, as all methods here are remapped to ComponentManager<T>

namespace DXH
{
class GameObject
{
    // GameObjects are not copyable, as they are managed by ComponentManagers
    GameObject(const GameObject&) = delete;
public:
    GameObject();
    ~GameObject();

    /// <summary>
    /// Adds a component to the game object.
    /// </summary>
    template <ComponentConcept T> T& Add()
    {
        m_ReleaseCallbacks.push_back([](GameObject* pObject)
            {
                ComponentManager<T>::GetInstance().Detach(pObject);
            });
        return ComponentManager<T>::GetInstance().Assign(this);
    }
    /// <summary>
    /// Gets a component from the game object.
    /// </summary>
    template <ComponentConcept T> inline T& Get() const
    {
        return ComponentManager<T>::GetInstance().GetComponent(this);
    }
    /// <summary>
    /// Removes a component from the game object.
    /// </summary>
    template <ComponentConcept T> inline void Remove()
    {
        ComponentManager<T>::GetInstance().Detach(this);
    }
    /// <summary>
    /// Checks if the game object has a component.
    /// </summary>
    template <ComponentConcept T> inline bool Has() const
    {
        return ComponentManager<T>::GetInstance().HasComponent(this);
    }

private:
    typedef void(*ReleaseCallback)(GameObject*);
    // A vector of callbacks that will be called when the game object is destroyed
    std::vector<ReleaseCallback> m_ReleaseCallbacks; 
};
}
