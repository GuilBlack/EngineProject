#pragma once
#include "src/ecs/components/Script.h" // Base class

namespace DXH
{
class BaseState;

/// <summary>
/// A state machine is a script that can change behaviour at runtime.
/// </summary>
class StateMachine : private Script
{
public:
    StateMachine(BaseState* currentState);
    ~StateMachine() = default;

    /// <summary>
    /// Updates the current state.
    /// </summary>
    inline void Update(const Timer& gt);

    /// <summary>
    /// Switches the current state to a new one.
    /// </summary>
    void SwitchState(BaseState* newState);

private:
    BaseState* m_currentState = nullptr;
};
}