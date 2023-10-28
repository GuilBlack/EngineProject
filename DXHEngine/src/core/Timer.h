#pragma once

namespace DXH
{
class Timer
{
public:
	Timer();

	// Starts the timer.
	void Start();
	// Stops/pauses the timer.
	void Stop();
	// Resets the timer.
	void Reset();
	// Call this every frame.
	void Tick();

	// Gets the time (in seconds) elapsed since the last frame.
	float DeltaTime() const;
	// Gets the total time (in seconds) elapsed since the timer started.
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
