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
};
}
