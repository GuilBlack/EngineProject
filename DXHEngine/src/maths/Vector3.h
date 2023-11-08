#pragma once

namespace DXH
{
struct Vector3 : public DirectX::XMFLOAT3
{
    /// <summary>
    /// Creates a new Vector3 with all of its components set to 0.
    /// </summary>
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(DirectX::FXMVECTOR v);
    /// <summary>
    /// Loads this Vector3 in a XMVECTOR.
    /// </summary>
    inline DirectX::XMVECTOR Load() const
    {
        return XMLoadFloat3(this);
    }
    /// <summary>
    /// Stores the given XMVECTOR in this Vector3.
    /// </summary>
    inline void Store(DirectX::FXMVECTOR v)
    {
        XMStoreFloat3(this, v);
    }

    inline DirectX::XMVECTOR Normalize() const
    {
        return DirectX::XMVector3Normalize(Load());
    }

    //////////////////////////////////////////////////////////////////////////
    // Static methods and variables //////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    inline static float Dot(Vector3 v1, Vector3 v2)
    {
        return DirectX::XMVectorGetX(DirectX::XMVector3Dot(v1.Load(), v2.Load()));
    }

    static const Vector3 Zero; // Shorthand for writing Vector3(0, 0, 0).
    static const Vector3 One; // Shorthand for writing Vector3(1, 1, 1).
    static const Vector3 Up; // Shorthand for writing Vector3(0, 1, 0).
    static const Vector3 Down; // Shorthand for writing Vector3(0, -1, 0).
    static const Vector3 Right; // Shorthand for writing Vector3(1, 0, 0).
    static const Vector3 Left; // Shorthand for writing Vector3(-1, 0, 0).
    static const Vector3 Forward; // Shorthand for writing Vector3(0, 0, 1).
    static const Vector3 Backward; // Shorthand for writing Vector3(0, 0, -1).

#pragma region Operators
    inline Vector3 operator+(const Vector3& v) const
    {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    inline void operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    inline Vector3 operator-(const Vector3& v) const
    {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    inline void operator-=(const Vector3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    inline Vector3 operator*(const float& f) const
    {
        return Vector3(x * f, y * f, z * f);
    }

    inline void operator*=(const float& f)
    {
        x *= f;
        y *= f;
        z *= f;
    }

    inline Vector3 operator/(const float& f) const
    {
        return Vector3(x / f, y / f, z / f);
    }

    inline void operator/=(const float& f)
    {
        x /= f;
        y /= f;
        z /= f;
    }
#pragma endregion
};
}
