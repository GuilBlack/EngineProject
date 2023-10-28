#pragma once

namespace DXH
{
struct Matrix : public DirectX::XMFLOAT4X4
{
	Matrix();
	Matrix(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44);
	XM_CALLCONV Matrix(DirectX::FXMMATRIX m);
	/// <summary>
	/// Loads this matrix in a XMMATRIX.
	/// </summary>
	inline DirectX::XMMATRIX XM_CALLCONV Load() const
	{
		return XMLoadFloat4x4(this);
	}
	/// <summary>
	/// Stores the given XMMATRIX in this matrix.
	/// </summary>
	inline void XM_CALLCONV Store(DirectX::FXMMATRIX m)
	{
		XMStoreFloat4x4(this, m);
	}

	static const Matrix Identity; // Shorthand for an identity matrix.

	inline XMMATRIX XM_CALLCONV operator*(DirectX::FXMMATRIX m) const { return XMMatrixMultiply(Load(), m); }
	inline void XM_CALLCONV operator*=(DirectX::FXMMATRIX m) { Store(XMMatrixMultiply(Load(), m)); }
	inline void operator=(DirectX::FXMMATRIX m) { Store(m); }
};
}
