#pragma once
#include "src/ecs/Component.h" // For the Component base class
#include "src/DXHMaths.h" // For math structs and functions

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
    Vector3 Position = Vector3::Zero;
    Quaternion Rotation = Quaternion::Identity;
    Vector3 Scale = Vector3::One;

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
