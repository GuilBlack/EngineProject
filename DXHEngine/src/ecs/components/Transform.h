#pragma once
#include "src/ecs/Component.h" // For the Component base class

namespace DXH
{
struct Transform : Component
{
	void Reset() override
	{
		Position = {0, 0, 0};
		Rotation = {0, 0, 0, 1};
		Scale = {1, 1, 1};
	}
	Vector3 Position = { 0,0,0 };
	Quaternion Rotation = { 0,0,0,1 };
	Vector3 Angles = { 0,0,0 };
	Vector3 Scale = { 1,1,1 };

	/// <summary>
	/// Gets the matrix describing the transformation from world space to model space.
	/// </summary>
	inline Matrix GetModelMatrix() const
	{
		using namespace DirectX;
		Matrix world = XMMatrixIdentity();

		world = XMMatrixMultiply(world, XMMatrixScalingFromVector(XMLoadFloat3(&Scale)));

		world = XMMatrixMultiply(world, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&Angles)));

		world = XMMatrixMultiply(world, XMMatrixTranslationFromVector(XMLoadFloat3(&Position)));

		return world;
	}
};
}
