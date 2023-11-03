#include "CameraRotator.h"
#include "DXHMaths.h"

using namespace DXH;

void CameraRotator::Start() 
{
	m_camTransform = &pGameObject->Get<Transform>();
	InputManager::GetInstance().ToggleCursorLock(true);
}


void CameraRotator::Update(const DXH::Timer& gt)
{
	Vector2 tMouseDelta = InputManager::GetInstance().GetMouseDelta();
	Vector3 tRotation = m_camTransform->Rotation.GetEulerAngles();
	tRotation.x += tMouseDelta.x;
	tRotation.y += tMouseDelta.y;

	//DirectX::XMVECTOR mouseRotation = DirectX::XMQuaternionRotationRollPitchYaw(m_pitch, m_yaw, m_roll);
	//DirectX::XMVECTOR newRotation = DirectX::XMQuaternionMultiply(m_camTransform->Rotation.Load(), mouseRotation);
	m_camTransform->Rotation.SetEulerAngles(tRotation);
}
