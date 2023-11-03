#pragma once
#include "Vector4.h"
#include "Vector3.h"

namespace DXH
{
struct Quaternion : public Vector4
{
    /// <summary>
    /// Creates an identity quaternion.
    /// </summary>
    Quaternion() : Vector4(0.0f, 0.0f, 0.0f, 1.0f) {}
    Quaternion(float x, float y, float z, float w) : Vector4(x, y, z, w) {}
    Quaternion(DirectX::FXMVECTOR v) : Vector4(v) {}

    // Shorthand for writing Quaternion(0, 0, 0, 1).
    static const Quaternion Identity;

    Vector3 GetEulerAngles() const
    {
        return Vector3(
            DirectX::XMConvertToDegrees(std::atan2(2.f * (x * y + w * z), 1.f - 2.f * (x * x + y * y))),
            DirectX::XMConvertToDegrees(std::asin(-2.f * (y * z - w * x))),
            DirectX::XMConvertToDegrees(std::atan2(2.f * (x * z + w * y), 1.f - 2.f * (y * y + z * z))));
    }

    inline void SetEulerAngles(float x, float y, float z)
    {
        Store(DirectX::XMQuaternionRotationRollPitchYaw(x, y, z));
    }

    inline void SetEulerAngles(Vector3 v)
    {
        Store(DirectX::XMQuaternionRotationRollPitchYawFromVector(v.Load()));
    }

    inline DirectX::XMMATRIX GetRotationMatrix() const
    {
        return DirectX::XMMatrixRotationQuaternion(Load());
    }
};
}
