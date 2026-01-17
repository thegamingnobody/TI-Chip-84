
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
	if (timer_ChkInterrupt(2, TIMER_RELOADED)){
		timer_AckInterrupt(2, TIMER_RELOADED);					//reset the interrupt

		m_AverageCyclesPerSecond *= m_AmountOfTimerInterrupts; 	//undo average calculations
		m_AverageFramesPerSecond *= m_AmountOfTimerInterrupts;
		
		m_AverageCyclesPerSecond += m_CyclesExecuted; 			//add current frame data
		m_AverageFramesPerSecond += m_FrameUpdateCount;
		
		m_AmountOfTimerInterrupts++;							//increment amount of samples taken

		m_AverageCyclesPerSecond /= m_AmountOfTimerInterrupts;	//calculate new averages
		m_AverageFramesPerSecond /= m_AmountOfTimerInterrupts;
		
		dbg_printf("  FPS: %f\n", m_AverageFramesPerSecond);
		dbg_printf("Cycle: %f\n", m_AverageCyclesPerSecond);
		dbg_printf("*---------*\n");

		m_CyclesExecuted = 0;
		m_FrameUpdateCount = 0;
	}
	
}


void TimeManager::LimitFrameRate()
{
	float targetFrameTime  = 1.0f / m_TargetFPS;

	clock_t now{ clock() };
	float frameTime{ (float)(now - m_CurrentTime) / CLOCKS_PER_SEC };

	if (frameTime >= targetFrameTime) return;

	uint16_t msToDelay{ (uint16_t)((targetFrameTime - frameTime) * 1000.0f) };
	if (msToDelay <= 0) return;

	delay(msToDelay);

	// dbg_printf("  targetFrameTime: %f\n", targetSecondsPerFrame);
	// dbg_printf("        frameTime: %f\n", frameTime);
	// dbg_printf("        remaining: %f\n", remaining);
	// dbg_printf("        msToDelay: %d\n", msToDelay);
	// dbg_printf("*-------------------*\n");
}

float TimeManager::CalculateInstructionToExecute() const
{
	return m_DeltaTime * m_TargetIPF * m_TargetFPS;
}

float TimeManager::CalculateTimerUpdates() const
{
	return m_DeltaTime * m_TargetFPS;
}