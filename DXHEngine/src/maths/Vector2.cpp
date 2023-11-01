#include "Vector2.h"
using namespace DirectX;

namespace DXH
{
Vector2::Vector2()
    : XMFLOAT2(0.0f, 0.0f)
{
}

Vector2::Vector2(float x, float y)
    : XMFLOAT2(x, y)
{
}

Vector2::Vector2(FXMVECTOR v)
{
    XMStoreFloat2(this, v);
}

const Vector2 Vector2::Zero(0.0f, 0.0f);
const Vector2 Vector2::One(1.0f, 1.0f);
const Vector2 Vector2::Up(0.0f, 1.0f);
const Vector2 Vector2::Down(0.0f, -1.0f);
const Vector2 Vector2::Right(1.0f, 0.0f);
const Vector2 Vector2::Left(-1.0f, 0.0f);
}
