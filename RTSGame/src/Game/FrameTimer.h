#pragma once

#include <SFML\Graphics.hpp>

class FrameTimer
{
public:
	FrameTimer();
	FrameTimer(const FrameTimer&) = delete;
	FrameTimer& operator=(const FrameTimer&) = delete;
	FrameTimer(FrameTimer&&) = delete;
	FrameTimer&& operator=(FrameTimer&&) = delete;

	void update();
	float getDeltaTime() const;

private:
	sf::Time m_timer;
	sf::Clock m_clock;
};