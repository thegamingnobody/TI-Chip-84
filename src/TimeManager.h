#pragma once

#include "Singleton.h"
#include <sys/timers.h>
#include <ctime>


class TimeManager: public Singleton<TimeManager>
{
public:
    void Init();

    void SetTimer();

    void UpdateTime(bool isGamePaused);

    // std::chrono::milliseconds GetSleepTime();

    float GetInstructionsPerSecond() const { return m_AverageCyclesPerSecond; }
    double GetAverageFPS() const { return m_AverageFramesPerSecond; }

    void IncrementCycleCounter() { m_CyclesExecuted++; }
    int GetTargetIPF() const { return m_TargetIPF; }
    int GetTargetFPS() const { return m_TargetFPS; }

    void IncrementFrameCounter() { m_FrameUpdateCount++; }

    float GetDeltaTime() const { return m_DeltaTime; }

    void LimitFrameRate();

private:
    float m_DeltaTime{0.0f};

    //Instructions Per Frame
    int m_TargetIPF;

    int m_TargetFPS;

    clock_t m_LastTime;
    clock_t m_CurrentTime;
    int m_CyclesExecuted;
    int m_FrameUpdateCount;
    double m_AverageCyclesPerSecond;
    double m_AverageFramesPerSecond{15};

    uint16_t const m_ClocksPerSecond{ 0x8000 };

    // std::chrono::milliseconds m_SleepTime;
};