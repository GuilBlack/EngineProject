#pragma once

namespace DXH
{
struct Quaternion : public DirectX::XMFLOAT4
{
	/// <summary>
	/// Creates an identity quaternion.
	/// </summary>
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(DirectX::FXMVECTOR v);
	/// <summary>
	/// Loads this quaternion in a XMVECTOR.
	/// </summary>
	inline DirectX::XMVECTOR Load() const
	{
		return XMLoadFloat4(this);
	}
	/// <summary>
	/// Stores the provided XMVECTOR in this quaternion.
	/// </summary>
	inline void Store(DirectX::FXMVECTOR v)
	{
		XMStoreFloat4(this, v);
	}

	static const Quaternion Identity; // Shorthand for writing Quaternion(0, 0, 0, 1).
};
}
