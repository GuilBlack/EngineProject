#pragma once

namespace DXH
{
class Timer
{
public:
    /// <summary>
    /// Create and start a new timer.
    /// </summary>
    Timer();
    /// <summary>
    /// Starts the timer.
    /// </summary>
    void Start();
    /// <summary>
    /// Stops/pauses the timer.
    /// </summary>
    void Stop();
    /// <summary>
    /// Resets and restarts the timer.
    /// </summary>
    void Reset();
    /// <summary>
    /// Call this to update all the timer values.
    /// </summary>
    void Tick();

public: // Const methods, will be available in const objects.
    /// <summary>
    /// Gets the time (in seconds) elapsed since the last frame.
    /// </summary>
    float DeltaTime() const;
    /// <summary>
    /// Gets the total time (in seconds) elapsed since the timer started.
    /// </summary>
    float TotalTime() const;

private:
    double m_SecondsPerCount;
    double m_DeltaTime;

    __int64 m_BaseTime;
    __int64 m_PausedTime;
    __int64 m_StopTime;
    __int64 m_PrevTime;
    __int64 m_CurrTime;

    bool m_IsStopped;
};
}
