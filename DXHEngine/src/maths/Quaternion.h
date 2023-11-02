#pragma once
#include "Vector4.h"

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

    inline void SetRotationFromAngles(float x, float y, float z)
    {
        Store(DirectX::XMQuaternionRotationRollPitchYaw(x, y, z));
    }

    inline DirectX::XMMATRIX GetRotationMatrix() const
    {
        return DirectX::XMMatrixRotationQuaternion(Load());
    }
};

const Quaternion Quaternion::Identity = Quaternion();
}
