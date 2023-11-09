#include "Button.h"
#include <DXHCore.h>
#include "../Game.h"
#include "../DXHEngine/src/core/Window.h"
using namespace DXH;

void Button::Update(const DXH::Timer& gt)
{
	Vector2 tmp = InputManager::GetInstance().GetNormalizeCoord();
	if (InputManager::GetKeyState(VK_LBUTTON) == KeyState::Pressed && IsInStart(tmp))
	{
		if (m_StartCallback != nullptr)
			m_StartCallback();
		pGameObject->Destroy();
	}
	if (InputManager::GetKeyState(VK_LBUTTON) == KeyState::Pressed && IsInExit(tmp))
	{
		if (m_ExitCallback != nullptr)
			m_ExitCallback();
		pGameObject->Destroy();
	}
}

bool Button::IsInStart(Vector2 tmp)
{
	if (tmp.x > -0.17f && tmp.x < 0.15f &&
		tmp.y < 0.60f && tmp.y > 0.01f)
	{
		return true; 
	}
	return false;
}

bool Button::IsInExit(Vector2 tmp)
{
	if (tmp.x > -0.17f && tmp.x < 0.15f &&
		tmp.y < -0.098f && tmp.y > -0.55f)
	{
		return true;
	}
	return false;
}