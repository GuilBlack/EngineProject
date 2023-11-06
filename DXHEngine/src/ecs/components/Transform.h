#pragma once
#include "src/ecs/Component.h" // For the Component base class
#include "DXHMaths.h" // For math structs and functions

namespace DXH
{
struct Transform : Component
{
    void OnAssign() override
    {
        Position = Vector3::Zero;
        Rotation = Quaternion::Identity;
        Scale = Vector3::One;
    }
    Vector3 Position = Vector3::Zero; // Position in world space, in meters
    int GridPosition[3] = { 0,0,0 }; // Position in the grid
    Quaternion Rotation = Quaternion::Identity;
    Vector3 Scale = Vector3::One;

    // Local axis
    Vector3 Up = Vector3::Up;
    Vector3 Right = Vector3::Right;
    Vector3 Forward = Vector3::Forward;

    void SetPosition(DirectX::FXMVECTOR& position) 
    { 
        using namespace DirectX;
        Position.Store(position); 
        GridPosition[0] = (int)(Position.x / GRID_SIZE);
        GridPosition[1] = (int)(Position.y / GRID_SIZE);
        GridPosition[2] = (int)(Position.z / GRID_SIZE);
    }
    
    /// <summary>
    /// Gets the matrix describing the transformation from world space to model space.
    /// </summary>
    inline DirectX::XMMATRIX GetModelMatrix() const
    {
        return DirectX::XMMatrixAffineTransformation(
            Scale.Load(),
            DirectX::XMVectorZero(),
            Rotation.Load(),
            Position.Load());
    }

    inline DirectX::XMMATRIX GetScaleMatrix() const
    {
        return DirectX::XMMatrixScalingFromVector(Scale.Load());
    }

    inline DirectX::XMMATRIX GetRotationMatrix() const
    {
        return DirectX::XMMatrixRotationQuaternion(Rotation.Load());
    }

    inline void SetEulerAngles(Vector3 rotation)
    {
        Rotation.SetEulerAngles(rotation);
        Matrix m = GetRotationMatrix();
        Up = { m._21,m._22,m._23 };
        Right = { m._11,m._12,m._13 };
        Forward = { m._31, m._32,m._33 };
    }
};
}
