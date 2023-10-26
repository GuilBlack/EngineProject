#include "InputManager.h"

void InputManager::CheckInputs()
{
	if (m_KeyState.empty())
	{
		m_KeyState.resize(m_Keys.size(), KeyState::NONE);
	}

	vector<KeyState> currentKeyState(m_Keys.size(), KeyState::NONE);

	for (size_t i = 0; i < m_Keys.size(); ++i)
	{
		if (GetAsyncKeyState(m_Keys[i]) < 0) {
			currentKeyState[i] = (m_KeyState[i] == KeyState::HELD || m_KeyState[i] == KeyState::DOWN) ? KeyState::HELD : KeyState::DOWN;
		}
		else
		{
			currentKeyState[i] = (m_KeyState[i] == KeyState::HELD || m_KeyState[i] == KeyState::DOWN) ? KeyState::UP : KeyState::NONE;
		}
	}
	m_KeyState = currentKeyState;
}
