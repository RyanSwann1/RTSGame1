#include <Game\FrameTimer.h>
#include <Locators\FrameTimerLocator.h>

FrameTimer::FrameTimer()
{
	FrameTimerLocator::provide(*this);
}

void FrameTimer::update()
{
	m_timer = m_clock.restart();
}

float FrameTimer::getDeltaTime() const
{
	return m_timer.asSeconds();
}