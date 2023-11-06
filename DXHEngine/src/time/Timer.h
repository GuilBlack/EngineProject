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
    /// <summary>
    /// Changes the time scale of the timer. This can be used to create slow motion effects.
    /// </summary>
    /// <param name="timeScale">The new time scale.</param>
    void SetTimeScale(float timeScale) { m_timeScale = timeScale; }

public: // Const methods, will be available in const objects.
    /// <summary>
    /// Gets the time (in seconds) elapsed since the last frame.
    /// </summary>
    float DeltaTime() const { return (float)m_DeltaTime; }
    /// <summary>
    /// Gets the time (in seconds) elapsed since the last frame.
    /// </summary>
    double PreciseDeltaTime() const { return m_DeltaTime; }
    /// <summary>
    /// Gets the total time (in seconds) elapsed since the timer started.
    /// </summary>
    float TotalTime() const;
    /// <summary>
    /// Gets the time scale of the timer.
    /// </summary>
    float TimeScale() const { return m_timeScale; }

private:
    double m_SecondsPerCount;
    double m_DeltaTime;

    __int64 m_BaseTime;
    __int64 m_PausedTime;
    __int64 m_StopTime;
    __int64 m_PrevTime;
    __int64 m_CurrTime;

    bool m_IsStopped;
    float m_timeScale;
};
}
