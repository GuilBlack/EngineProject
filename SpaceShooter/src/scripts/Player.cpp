#include "Player.h"
#include "DXHScripting.h"

using namespace DXH;

void Player::Start()
{
	m_camTransform = &pGameObject->Get<Transform>();
	InputManager::GetInstance().ToggleCursorLock(true);
}

void Player::Update(const DXH::Timer& gt)
{
	Vector2 tMouseDelta = InputManager::GetInstance().GetMouseDelta();
	Vector3 tRotation = m_camTransform->Rotation.GetEulerAngles();
	tRotation.x += tMouseDelta.x * m_MouseSensitivity;
	tRotation.y += tMouseDelta.y * m_MouseSensitivity;
	tRotation.z = 0;
	m_camTransform->Rotation.SetEulerAngles(tRotation);
}