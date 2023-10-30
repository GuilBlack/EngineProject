#pragma once
#include "src/ecs/Component.h" // For the Component base class
#include "src/DXHMaths.h" // For math structs and functions

namespace DXH
{
struct Transform : Component
{
	void Reset() override
	{
		Position = Vector3::Zero;
		Rotation = Quaternion::Identity;
		Scale = Vector3::One;
	}
	Vector3 Position = { 0,0,0 };
	Quaternion Rotation = { 0,0,0,1 };
	Vector3 Scale = { 1,1,1 };

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
};
}
