#pragma once
#include <ctime>

class CTimer
{
private:
	int64_t m_startTime;
	int64_t m_lastDelta;
	int64_t m_lastTime;
	int64_t m_frequency;
	float m_gameScale;

public:
	CTimer()
		: m_lastDelta(0)
		, m_gameScale(1.f)
	{
		QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>(&m_frequency) );
		QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>(&m_startTime) );
		m_lastTime = m_startTime;
		m_frequency *= 1000000;
	}

	void SetGameScale(float const scale)
	{
		m_gameScale = scale;
	}

	void Tick()
	{
		int64_t const prevTime = m_lastTime;
		QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>(&m_lastTime) );
		m_lastDelta = m_lastTime - prevTime;
	}

	int64_t TimeFromStart() const
	{
		int64_t currentTime = 0;
		QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>(&currentTime) );
		return currentTime - m_startTime;
	}

	int64_t LastDelta() const
	{
		return m_lastDelta;
	}

	float Delta() const
	{
		int64_t const lastDeltaUS = m_lastDelta * 1000000;
		return static_cast<float>(lastDeltaUS) / static_cast<float>(m_frequency);
	}

	float GameDelta() const
	{
		return m_gameScale * Delta();
	}

	float GetSeconds( int64_t const time ) 
	{ 
		int64_t const timeUS = time * 1000000;
		return static_cast<float>(timeUS) / static_cast<float>(m_frequency);
	}
};

extern CTimer GTimer;