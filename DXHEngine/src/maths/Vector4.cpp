#include "Vector4.h"
using namespace DirectX;

namespace DXH
{
Vector4::Vector4()
    : XMFLOAT4()
{
}

Vector4::Vector4(float x, float y, float z, float w)
    : XMFLOAT4(x, y, z, w)
{
}

Vector4::Vector4(FXMVECTOR v)
{
    XMStoreFloat4(this, v);
}

const Vector4 Vector4::Zero = Vector4(0, 0, 0, 0);
const Vector4 Vector4::One = Vector4(1, 1, 1, 1);
}