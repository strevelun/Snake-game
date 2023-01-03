#pragma once

#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

class CFrameTime
{
	static CFrameTime* m_inst;

	DWORD m_tick = 0;
	float m_deltaTime = 0;
	int m_frame = 0;
	float m_elapsedTime = 0;
	float m_fps = 0;
	int m_frameLimit = 0;
	float m_time = 0;

public:
	static CFrameTime* GetInstance()
	{
		if (m_inst == nullptr)  m_inst = new CFrameTime();
		return m_inst;
	}

	CFrameTime();
	~CFrameTime();

	void SetFrameLimit(int _limit) { m_frameLimit = _limit; }

	float GetDeltaTime() const { return m_deltaTime; }
	int GetFps() const { return m_fps; }
	bool Update();

};

