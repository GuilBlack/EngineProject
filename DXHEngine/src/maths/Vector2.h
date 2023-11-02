#pragma once

namespace DXH
{
struct Vector2 : public DirectX::XMFLOAT2
{
    /// <summary>
    /// Creates a new Vector2 with both its components set to 0.
    /// </summary>
    Vector2();
    Vector2(float x, float y);
    Vector2(DirectX::FXMVECTOR v);
    /// <summary>
    /// Loads this Vector3 in a XMVECTOR.
    /// </summary>
    inline DirectX::XMVECTOR Load() const
    {
        return XMLoadFloat2(this);
    }
    /// <summary>
    /// Stores the given XMVECTOR in this Vector3.
    /// </summary>
    inline void Store(DirectX::FXMVECTOR v)
    {
        XMStoreFloat2(this, v);
    }

    static const Vector2 Zero; // Shorthand for writing Vector2(0, 0)
    static const Vector2 One; // Shorthand for writing Vector2(1, 1)
    static const Vector2 Up; // Shorthand for writing Vector2(0, 1)
    static const Vector2 Down; // Shorthand for writing Vector2(0, -1)
    static const Vector2 Right; // Shorthand for writing Vector2(1, 0)
    static const Vector2 Left; // Shorthand for writing Vector2(-1, 0)
};
}
