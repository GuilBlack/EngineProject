#pragma once
#include "src/ecs/Component.h" // For the Component base class

namespace DXH
{
struct Transform : Component
{
	void Reset() override
	{
		position = Vector3::Zero;
		rotation = Quaternion::Identity;
		scale = Vector3::One;
	}
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	/// <summary>
	/// Gets the matrix describing the transformation from world space to model space.
	/// </summary>
	inline DirectX::XMMATRIX GetModelMatrix() const
	{
		return DirectX::XMMatrixAffineTransformation(
			XMLoadFloat3(&scale),
			DirectX::XMVectorZero(),
			XMLoadFloat4(&rotation),
			XMLoadFloat3(&position));
	}
};
}
