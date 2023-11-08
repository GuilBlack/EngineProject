#include "GameObject.h"
#include "systems/GameObjectCollector.h"
using namespace DirectX;

namespace DXH
{
GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

GameObject* GameObject::Create()
{
    GameObject* obj = new GameObject();
    GameObjectCollector::GetInstance().AddGameObject(obj);
    return obj;
}

void GameObject::Destroy()
{
    // Call all the destroy callbacks
    for (auto& callback : m_ReleaseCallbacks)
        callback(this);
    m_ReleaseCallbacks.clear();

    // Remove from the game object manager
    GameObjectCollector::GetInstance().RemoveGameObject(this);
    delete this;
}

void GameObject::SetPosition(Vector3 position)
{
    m_Position = position;
    // Update the grid position
    m_GridPosition[0] = (int)(position.x / GRID_SIZE);
    m_GridPosition[1] = (int)(position.y / GRID_SIZE);
    m_GridPosition[2] = (int)(position.z / GRID_SIZE);
}

void GameObject::SetRotation(Quaternion rotation)
{
    m_Rotation = rotation;
    Matrix m = GetRotationMatrix();
    m_Up = {m._21,m._22,m._23};
    m_Right = {m._11,m._12,m._13};
    m_Forward = {m._31, m._32,m._33};
}

void GameObject::SetEulerAngles(Vector3 rotation)
{
    m_Rotation.SetEulerAngles(rotation);
    Matrix m = GetRotationMatrix();
    m_Up = {m._21,m._22,m._23};
    m_Right = {m._11,m._12,m._13};
    m_Forward = {m._31, m._32,m._33};
}

void GameObject::Rotate(Quaternion rotation)
{
    XMMATRIX xm = m_Rotation.GetRotationMatrix() * rotation.GetRotationMatrix();
    m_Rotation.Store(XMQuaternionRotationMatrix(xm));
    Matrix m = xm;
    m_Up = {m._21,m._22,m._23};
    m_Right = {m._11,m._12,m._13};
    m_Forward = {m._31, m._32,m._33};

}

void GameObject::Rotate(Vector3 rotation)
{
    Quaternion q;
    q.SetEulerAngles(rotation);
    Rotate(q);
}
}
