#pragma once

#include "Singleton.h"
#include <sys/timers.h>
#include <TINYSTL/string.h>

class TimeManager: public Singleton<TimeManager>
{
public:
    void Init();
    void UpdateTime(bool isGamePaused);

    // std::chrono::milliseconds GetSleepTime();

    float GetInstructionsPerSecond() const { return m_AverageCyclesPerSecond; }

    void IncrementCycleCounter() { m_CyclesExecuted++; }
    int GetInstructionPerFrame() const { return m_TargetIPF; }

    void IncrementFrameCounter() { m_FrameUpdateCount++; }

    void RenderImGui(tinystl::string windowName);
private:
    float m_DeltaTime;

    //Instructions Per Frame
    int m_TargetIPF;

    int m_TargetFPS;

    // std::chrono::high_resolution_clock::time_point m_LastTime;
    // std::chrono::high_resolution_clock::time_point m_CurrentTime;
    int m_CyclesExecuted;
    int m_FrameUpdateCount;
    float m_ImguiStatsUpdateDelay;
    float m_ImguiStatsUpdateMaxDelay;
    double m_AverageCyclesPerSecond;
    double m_AverageFramesPerSecond;

    // std::chrono::milliseconds m_SleepTime;
};