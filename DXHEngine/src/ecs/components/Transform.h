#pragma once
#include "src/ecs/Component.h"

namespace DXH
{
struct Transform : Component
{
	void Reset() override
	{
		position = {0, 0, 0};
		rotation = {0, 0, 0, 1};
		scale = {1, 1, 1};
	}
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	/// <summary>
	/// Gets the matrix describing the transformation from world space to model space.
	/// </summary>
	inline Matrix GetModelMatrix() const
	{
		return DirectX::XMMatrixAffineTransformation(
			XMLoadFloat3(&scale),
			DirectX::XMVectorZero(),
			XMLoadFloat4(&rotation),
			XMLoadFloat3(&position));
	}
};
}
