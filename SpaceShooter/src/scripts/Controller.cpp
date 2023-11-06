#include "Controller.h"
using namespace DXH;

void Controller::Start()
{
    m_camTransform = &pGameObject->Get<Transform>();
    InputManager::GetInstance().ToggleCursorLock(true);
}

void Controller::Update(const DXH::Timer& gt)
{
    if (InputManager::IsKeyPressed(VK_ESCAPE))
        InputManager::ToggleCursorLock();

    Vector2 tMouseDelta = InputManager::GetInstance().GetMouseDelta();
    // Don't forget to put mouseDelta X in rotation Y, and mouseDelta Y in rotation X!
    m_rotation.x -= tMouseDelta.y * m_MouseSensitivity;
    m_rotation.y += tMouseDelta.x * m_MouseSensitivity;
    // Limit the rotation of the camera so it doesn't do a backflip
    if (m_rotation.x > 89.0f) m_rotation.x = 89.0f;
    if (m_rotation.x < -89.0f) m_rotation.x = -89.0f;
    m_camTransform->Rotation.SetEulerAngles(m_rotation);
}
