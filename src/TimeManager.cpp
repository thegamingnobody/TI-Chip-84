
#include "TimeManager.h"
#include <debug.h>

TimeManager::TimeManager() 
	: m_TargetIPF(11)
	, m_TargetFPS(60)
	, m_CyclesExecuted(0)
	, m_FrameUpdateCount(0)
{
	timer_SetReload(2, m_ClocksPerSecond);
	m_LastTime = clock();
	m_CurrentTime = clock();
}

void TimeManager::Init()
{
	// m_DeltaTime = 0.0f;

	m_TargetFPS = 60;

	m_TargetIPF = 11;

	m_CyclesExecuted = 0;

	timer_SetReload(2, m_ClocksPerSecond);
	m_LastTime = clock();
	m_CurrentTime = clock();
}

void TimeManager::SetTimer()
{
	timer_Set(2, m_ClocksPerSecond);
	timer_Enable(2, TIMER_32K, TIMER_0INT, TIMER_DOWN);
	m_LastTime = clock();
}

void TimeManager::UpdateTime(bool isGamePaused)
{
	m_CurrentTime = clock();
	m_DeltaTime = (float)(m_CurrentTime - m_LastTime) / CLOCKS_PER_SEC;
	m_LastTime = m_CurrentTime;

	if (isGamePaused) return;

	//check if timer is done
	if (timer_ChkInterrupt(2, TIMER_RELOADED))
	{
		//reset the interrupt
		timer_AckInterrupt(2, TIMER_RELOADED);
		m_AverageCyclesPerSecond = (m_CyclesExecuted);
		m_AverageFramesPerSecond = (m_FrameUpdateCount);

		m_CyclesExecuted = 0;
		m_FrameUpdateCount = 0;

		dbg_printf("%f\n", m_AverageFramesPerSecond);
	}
	
}


void TimeManager::LimitFrameRate()
{
	float targetSecondsPerFrame = (1.0f / m_TargetFPS);
	
	float remaining = targetSecondsPerFrame - m_DeltaTime;
	
    if (remaining <= 0.0f) return;
	
    delay(remaining * 1000);
	
}