#include "Timer.h"

namespace DXH
{
Timer::Timer()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    m_SecondsPerCount = 1.0 / frequency.QuadPart;
    Reset();
}

void Timer::Start()
{
    if (m_IsStopped)
    {
        LARGE_INTEGER currTime;
        QueryPerformanceCounter(&currTime);

        m_PausedTime += (currTime.QuadPart - m_StopTime);
        m_PrevTime = currTime.QuadPart;
        m_StopTime = 0;
        m_IsStopped = false;
    }
}

void Timer::Stop()
{
    if (!m_IsStopped)
    {
        LARGE_INTEGER currTime;
        QueryPerformanceCounter(&currTime);

        m_StopTime = currTime.QuadPart;
        m_IsStopped = true;
    }
}

void Timer::Reset()
{
    LARGE_INTEGER currTime;
    QueryPerformanceCounter(&currTime);

    m_DeltaTime = -1.0;
    m_BaseTime = currTime.QuadPart;
    m_PrevTime = currTime.QuadPart;
    m_CurrTime = currTime.QuadPart;
    m_PausedTime = 0;
    m_StopTime = 0;
    m_IsStopped = false;
}

void Timer::Tick()
{
    if (m_IsStopped)
    {
        m_DeltaTime = 0.0;
        return;
    }

    LARGE_INTEGER currTime;
    QueryPerformanceCounter(&currTime);
    m_CurrTime = currTime.QuadPart;
    m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;
    m_PrevTime = m_CurrTime;

    // Force nonnegative. (Under certains circumstances, delta time can be negative.)
    if (m_DeltaTime < 0.0) m_DeltaTime = 0.0;
}

float Timer::TotalTime() const
{
    if (m_IsStopped)
    {
        return (float)((m_StopTime - m_PausedTime - m_BaseTime) * m_SecondsPerCount);
    }
    else
    {
        return (float)((m_CurrTime - m_PausedTime - m_BaseTime) * m_SecondsPerCount);
    }
}

float Timer::DeltaTime() const
{
    return (float)m_DeltaTime;
}
}
