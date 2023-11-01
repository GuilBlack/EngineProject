#pragma once

namespace DXH
{
struct Vector4 : public DirectX::XMFLOAT4
{
	/// <summary>
	/// Creates a new Vector2 with both its components set to 0.
	/// </summary>
	Vector4() : XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) {}
	Vector4(float x, float y, float z, float w)
		: XMFLOAT4(x, y, z, w) {}
	Vector4(DirectX::FXMVECTOR v)
	{
		DirectX::XMStoreFloat4(this, v);
	}
	/// <summary>
	/// Loads this Vector3 in a XMVECTOR.
	/// </summary>
	inline DirectX::XMVECTOR Load() const
	{
		return XMLoadFloat4(this);
	}
	/// <summary>
	/// Stores the given XMVECTOR in this Vector3.
	/// </summary>
	inline void Store(DirectX::FXMVECTOR v)
	{
		XMStoreFloat4(this, v);
	}
};
}