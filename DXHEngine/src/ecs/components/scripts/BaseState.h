#pragma once

namespace DXH
{
/// <summary>
/// Inherit from this class to create a state for a state machine.
/// </summary>
class BaseState
{
public:
    virtual ~BaseState() {};
    /// <summary>
    /// Called when the state is entered.
    /// </summary>
    virtual void EnterState() {}
    /// <summary>
    /// Called every frame.
    /// </summary>
    virtual void Update(const Timer& gt) {}
    /// <summary>
    /// Called when the state is left.
    /// </summary>
    virtual void LeaveState() {}
};
}
