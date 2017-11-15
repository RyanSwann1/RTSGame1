#pragma once

#include <assert.h>

class FrameTimer;
class FrameTimerLocator
{
public:
	static void provide(FrameTimer& frameTimer)
	{
		m_frameTimer = &frameTimer;
	}

	static FrameTimer& getFrameTimer()
	{
		assert(m_frameTimer);
		return *m_frameTimer;
	}

private:
	static FrameTimer* m_frameTimer;
};