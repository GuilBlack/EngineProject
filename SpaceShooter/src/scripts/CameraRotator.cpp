#include "CameraRotator.h"
#include "src/core/InputManager.h"
#include "src/ecs/components/Camera.h"


using namespace DXH;

void CameraRotator::Start() 
{
	m_camTransform = &pGameObject->Get<Transform>();
}


void CameraRotator::Update(const DXH::Timer& gt)
{
	m_pitch = InputManager::GetInstance().GetMousePosition().x ;
	m_MouseY = InputManager::GetInstance().GetMousePosition().y ;

	m_pitch = m_MouseX;
	m_yaw = m_MouseY;

	//DirectX::XMVECTOR mouseRotation = DirectX::XMQuaternionRotationRollPitchYaw(m_pitch, m_yaw, m_roll);
	//DirectX::XMVECTOR newRotation = DirectX::XMQuaternionMultiply(m_camTransform->Rotation.Load(), mouseRotation);
	m_camTransform->Rotation.SetRotationFromAngles(m_yaw, m_pitch, m_roll);
}
