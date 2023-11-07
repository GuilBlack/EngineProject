#include "CameraController.h"

using namespace DXH;

void CameraController::Start()
{
    m_SpaceshipRigibody = &pGameObject->Get<RigidBody>();
    InputManager::GetInstance().ToggleCursorLock(true);
}

void CameraController::Update(const DXH::Timer& gt)
{
    if (InputManager::GetKeyState(VK_ESCAPE) == KeyState::JustPressed)
        InputManager::ToggleCursorLock();

    UpdateCameraPosition();
}

void CameraController::UpdateCameraPosition()
{
    Vector2 tMouseDelta = InputManager::GetInstance().GetMouseDelta();
    // Don't forget to put mouseDelta X in rotation Y, and mouseDelta Y in rotation X!
    m_rotation.x -= tMouseDelta.y * m_MouseSensitivity;
    m_rotation.y += tMouseDelta.x * m_MouseSensitivity;
    // Limit the rotation of the camera so it doesn't do a backflip
    if (m_rotation.x > 89.0f) m_rotation.x = 89.0f;
    if (m_rotation.x < -89.0f) m_rotation.x = -89.0f;
    pGameObject->SetEulerAngles(m_rotation);
}