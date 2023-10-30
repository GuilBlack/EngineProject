#include "Vector3.h"
using namespace DirectX;

namespace DXH
{
Vector3::Vector3()
	: XMFLOAT3(0.0f, 0.0f, 0.0f)
{
}

Vector3::Vector3(float x, float y, float z)
	: XMFLOAT3(x, y, z)
{
}

Vector3::Vector3(FXMVECTOR v)
{
	XMStoreFloat3(this, v);
}

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Backward(0.0f, 0.0f, -1.0f);
}
