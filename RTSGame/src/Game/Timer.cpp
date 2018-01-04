#include <Game\Timer.h>

Timer::Timer(float expirationTime)
	: m_expirationTime(expirationTime),
	m_time(0)
{}

bool Timer::isExpired() const
{
	return m_time >= m_expirationTime;
}

void Timer::update(float deltaTime)
{
	m_time += deltaTime;
}

void Timer::reset()
{
	m_time = 0;
}