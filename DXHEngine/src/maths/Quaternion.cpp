#include "Quaternion.h"
using namespace DirectX;

namespace DXH
{
Quaternion::Quaternion()
	: XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Quaternion::Quaternion(float x, float y, float z, float w)
	: XMFLOAT4(x, y, z, w)
{
}

XM_CALLCONV Quaternion::Quaternion(FXMVECTOR v)
{
	XMStoreFloat4(this, v);
}

const Quaternion Quaternion::Identity = Quaternion();
}
