#pragma once

class Timer
{
public:
	Timer(float expirationTime);
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer&& operator=(Timer&&) = delete;

	bool isExpired() const;

	void update(float deltaTime);
	void reset();

private:
	const float m_expirationTime;
	float m_time;
};