#pragma once
#include <vector>
using namespace std;

class InputManager {
public:
	enum class KeyState {
		HELD,
		UP,
		DOWN,
		NONE,
	};
public:
	InputManager();

	void CheckInputs();

private:
	vector<char> m_Keys = {
		'q', 's','z','d',VK_LBUTTON,VK_RBUTTON
	};

	vector<KeyState> m_KeyState;
};