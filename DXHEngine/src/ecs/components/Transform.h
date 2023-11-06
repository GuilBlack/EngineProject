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
    Vector3 GridPosition = Vector3::Zero; // Position in grid space, in meters
    Quaternion Rotation = Quaternion::Identity;
    Vector3 Scale = Vector3::One;

    void SetPosition(DirectX::FXMVECTOR& position) 
    { 
        using namespace DirectX;
        Position.Store(position); 
        GridPosition.Store(position * (1 / GRID_SIZE));
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
};
}
