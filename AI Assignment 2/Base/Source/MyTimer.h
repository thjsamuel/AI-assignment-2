#ifndef MY_TIMER_H
#define MY_TIMER_H

#pragma comment(lib, "winmm.lib")

#include <Windows.h>

class CMyTimer
{
public:
	static CMyTimer* GetInstance()
	{
		static CMyTimer instance;

		return &instance;
	}

	// Returns how much time has elapsed since timer was started
	double GetCurrentTime_()
	{
		return timeGetTime() * 0.001 - m_dStartTime;
	}

private:
	CMyTimer()
	{
		// Starts timer when created
		m_dStartTime = timeGetTime() * 0.001;
	}

	double m_dStartTime; // seconds
};

#endif // MY_TIMER_H