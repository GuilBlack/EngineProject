#include "StateMachine.h"
#include "BaseState.h"

namespace DXH
{
StateMachine::StateMachine(BaseState* currentState)
    : m_currentState(currentState)
{
    currentState->EnterState();
}

inline void StateMachine::Update(const Timer& gt)
{
    m_currentState->Update(gt);
}

void StateMachine::SwitchState(BaseState* newState)
{
    m_currentState->LeaveState();
    m_currentState = newState;
    m_currentState->EnterState();
}
}