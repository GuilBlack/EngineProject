#include "Matrix.h"
using namespace DirectX;

namespace DXH
{
Matrix::Matrix()
	: XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
{
}

XM_CALLCONV Matrix::Matrix(FXMMATRIX m)
{
	XMStoreFloat4x4(this, m);
}

Matrix::Matrix(float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
	: XMFLOAT4X4(m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44)
{
}

const Matrix Matrix::Identity = Matrix();
}
