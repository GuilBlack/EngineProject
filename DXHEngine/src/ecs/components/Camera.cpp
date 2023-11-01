#include "Camera.h"
namespace DXH {

	using namespace DirectX;

	Matrix Camera::GetViewMatrix() const
	{
		return View;
	}
	Matrix Camera::GetViewProjectionMatrix() const
	{
		XMMATRIX tView = XMLoadFloat4x4(&View);
		XMMATRIX tProj = XMLoadFloat4x4(&Proj);
		return tView * tProj;
	}
}

