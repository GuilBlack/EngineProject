#pragma once
#include "ComponentManager.h" // Mandatory include, as all methods here are remapped to ComponentManager<T>
#include "DXHMaths.h" // For math structs and functions

namespace DXH
{
/// <summary>
/// A game object is a container for components and has a position, rotation and scale.
/// </summary>
class GameObject
{
    // GameObjects are not copyable, as they are managed by ComponentManagers
    GameObject(const GameObject&) = delete;

    friend class GameObjectCollector;
    GameObject();
    ~GameObject();
public:

    // Creates a new game object, will be automatically deleted if you don't destroy it yourself.
    static GameObject* Create();
    // Removes all components and destroys the game object.
    void Destroy();

#pragma region Component Manager Remapping
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
    template <ComponentConcept T> inline T& Get()
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
    template <ComponentConcept T> inline bool Has()
    {
        return ComponentManager<T>::GetInstance().HasComponent(this);
    }
#pragma endregion

    Vector3 Position() const { return m_Position; }
    void SetPosition(Vector3 position);
    void SetPosition(float x, float y, float z) { SetPosition({x,y,z}); }
    void Move(Vector3 offset) { SetPosition(m_Position + offset); }
    void Move(float x, float y, float z) { Move({x,y,z}); }
    Quaternion Rotation() const { return m_Rotation; }
    void SetRotation(Quaternion rotation);
    void SetEulerAngles(Vector3 rotation);
    void SetEulerAngles(float x, float y, float z) { SetEulerAngles({x,y,z}); }
    void Rotate(Quaternion rotation);
    void Rotate(Vector3 rotation);
    void Rotate(float x, float y, float z) { Rotate({x,y,z}); }
    Vector3 Scale() const { return m_Scale; }
    void SetScale(Vector3 scale) { m_Scale = scale; }
    void SetScale(float x, float y, float z) { SetScale({x,y,z}); }

    /// <summary>
    /// Gets the matrix describing the transformation from world space to model space.
    /// </summary>
    inline DirectX::XMMATRIX GetModelMatrix() const
    {
        return DirectX::XMMatrixAffineTransformation(
            m_Scale.Load(),
            DirectX::XMVectorZero(),
            m_Rotation.Load(),
            m_Position.Load());
    }
    inline DirectX::XMMATRIX GetTranslationMatrix() const { return DirectX::XMMatrixTranslationFromVector(m_Position.Load()); }
    inline DirectX::XMMATRIX GetRotationMatrix() const { return m_Rotation.GetRotationMatrix(); }
    inline DirectX::XMMATRIX GetScaleMatrix() const { return DirectX::XMMatrixScalingFromVector(m_Scale.Load()); }
    /// <summary>
    /// Check if this transform is near another transform (within 1 grid unit)
    /// </summary>
    bool IsNear(const GameObject& other) const
    {
        return abs(m_GridPosition[0] - other.m_GridPosition[0]) <= 1 &&
            abs(m_GridPosition[1] - other.m_GridPosition[1]) <= 1 &&
            abs(m_GridPosition[2] - other.m_GridPosition[2]) <= 1;
    }

    void GetLocalAxis(Vector3& up, Vector3& right, Vector3& forward) const
    {
        up = m_Up;
        right = m_Right;
        forward = m_Forward;
    }

private:
    typedef void(*ReleaseCallback)(GameObject*);
    // A vector of callbacks that will be called when the game object is destroyed
    std::vector<ReleaseCallback> m_ReleaseCallbacks;

    Vector3 m_Position = Vector3::Zero; // Position in world space, in meters
    int m_GridPosition[3] = {0,0,0}; // Position in the grid
    Quaternion m_Rotation = Quaternion::Identity;
    Vector3 m_Scale = Vector3::One;

    // Local axis
    Vector3 m_Up = Vector3::Up;
    Vector3 m_Right = Vector3::Right;
    Vector3 m_Forward = Vector3::Forward;
};
}
