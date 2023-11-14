#include "Timer.h"

Timer::Timer()
{
	m_elapedTime = 0.0f;
}

Timer::~Timer()
{
}

void Timer::SetTimer(float elapedTime)
{
	m_elapedTime = elapedTime;
}


