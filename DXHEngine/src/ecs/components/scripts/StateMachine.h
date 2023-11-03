#pragma once
#include "src/ecs/components/Script.h"
#include "BaseState.h"

namespace DXH
{
/// <summary>
/// A state machine is a script that can change behaviour at runtime.
/// </summary>
class StateMachine : private Script
{
public:
    StateMachine(BaseState* currentState)
        : m_currentState(currentState)
    {
        currentState->EnterState();
    }

    /// <summary>
    /// Updates the current state.
    /// </summary>
    inline void Update(const Timer& gt) { m_currentState->Update(gt); }

    /// <summary>
    /// Switches the current state to a new one.
    /// </summary>
    void SwitchState(BaseState* newState)
    {
        m_currentState->LeaveState();
        m_currentState = newState;
        m_currentState->EnterState();
    }

private:
    BaseState* m_currentState = nullptr;
};
}