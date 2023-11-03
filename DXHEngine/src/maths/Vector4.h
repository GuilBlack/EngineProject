#pragma once

namespace DXH
{
struct Vector4 : public DirectX::XMFLOAT4
{
    /// <summary>
    /// Creates a new Vector2 with both its components set to 0.
    /// </summary>
    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(DirectX::FXMVECTOR v);
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

    static const Vector4 Zero; // Shorthand for writing Vector4(0, 0, 0, 0)
    static const Vector4 One; // Shorthand for writing Vector4(1, 1, 1, 1)
};
}