
#include "TimeManager.h"

void TimeManager::Init()
{
	m_DeltaTime = 0.0f;

	m_TargetFPS = 60;

	m_TargetIPF = 11;

	// m_LastTime = std::chrono::high_resolution_clock::now();
	// m_CurrentTime = std::chrono::high_resolution_clock::now();

	m_CyclesExecuted = 0;
}

void TimeManager::UpdateTime(bool isGamePaused)
{
	// m_CurrentTime = std::chrono::high_resolution_clock::now();
	// m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_LastTime).count();
	// m_LastTime = m_CurrentTime;

	// if (isGamePaused) return;


	// if (m_ImguiStatsUpdateDelay >= m_ImguiStatsUpdateMaxDelay)
	// {
	// 	m_AverageCyclesPerSecond = (m_CyclesExecuted / m_ImguiStatsUpdateDelay);
	// 	m_AverageFramesPerSecond = (m_FrameUpdateCount / m_ImguiStatsUpdateDelay);
	// 	m_CyclesExecuted = 0;
	// 	m_FrameUpdateCount = 0;

	// 	m_ImguiStatsUpdateDelay -= m_ImguiStatsUpdateMaxDelay;
	// }

}
