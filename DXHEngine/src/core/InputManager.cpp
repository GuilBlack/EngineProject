#include "InputManager.h"

void InputManager::CheckInputs()
{
	if (m_KeyState.empty())
	{
		m_KeyState.resize(m_Keys.size(), KeyState::NONE);
	}

	vector<KeyState> currentKeyState(m_Keys.size(), KeyState::NONE);
}
