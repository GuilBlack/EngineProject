#include "Player.h"
#include "src/core/InputManager.h"
#include "src/ecs/components/Camera.h"

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
	tRotation.x += tMouseDelta.x;
	tRotation.y += tMouseDelta.y;

	m_camTransform->Rotation.SetEulerAngles(tRotation);
}