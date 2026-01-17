//-std=c++17

/*
 *--------------------------------------
 * Program Name:
 * Author:
 * License:
 * Description:
 *--------------------------------------
*/

/* Other available headers */
#include <graphx.h>
#include <fileioc.h>
#include <keypadc.h>
#include <intce.h>
#include <tice.h>
#include <math.h>
#include <fontlibc.h>

#include "Renderer.h"
#include "TypeDefinitions.h"
#include "Interpreter.h"
#include "GameLoader.h"
#include <TINYSTL/string.h>
#include "InputManager.h"
#include "TimeManager.h"
#include <debug.h>
#include "QuirkManager.h"

tinystl::string HandleGameChoice();
void ExecuteInstructions(Interpreter& interpreter, TimeManager& timeManager, float& amountOfInstructionsToExecute);
void UpdateTimers(Interpreter& interpreter, float& amountOfUpdates);
void TryRender(Interpreter& interpreter);
void EndFrame(TimeManager& timeManager);

int main(void)
{
    int constexpr canvasX{VIEWPORT_WIDTH_BASE};
    int constexpr canvasY{VIEWPORT_HEIGHT_BASE + 40}; // 32 as screen height and 40 to center it vertically
    int constexpr renderScale{3};

    gfx_Begin();

    tinystl::string gameName = HandleGameChoice();

    gfx_SetColor(0xFF);    
    gfx_FillScreen(0xFF);
    
    auto& renderer = Renderer::GetInstance();
    auto& inputManager = InputManager::GetInstance();
    auto& quirkManager = QuirkManager::GetInstance();
    auto timeManager = TimeManager();
    
    renderer.Init(canvasX, canvasY, renderScale);
    inputManager.Init();
    timeManager.Init();
    quirkManager.Init();
    auto interpreter = Interpreter();
      
    interpreter.LoadGame(gameName.c_str());

    timeManager.SetTimer();
    
    float timerUpdates{0};
    float instructionToExecute{0};
    bool continueRunning{ true };
    while (continueRunning){     
        timeManager.UpdateTime(false);       

        continueRunning = inputManager.ProcessInput();
        
        timerUpdates += timeManager.CalculateTimerUpdates();
        UpdateTimers(interpreter, timerUpdates);
        
        instructionToExecute += timeManager.CalculateInstructionToExecute();
        ExecuteInstructions(interpreter, timeManager, instructionToExecute);
        
        TryRender(interpreter);
        
        EndFrame(timeManager);
    }

    inputManager.Destroy();

    gfx_End();

    return 0;
}

tinystl::string HandleGameChoice()
{
    auto gameLoader = GameLoader();
    
    gameLoader.ListChip8Games();
    
    tinystl::string gameName;
    bool gameSelected{false};
    while(!gameSelected)
    {
        uint8_t key = os_GetCSC();

        switch (key)
        {
        case sk_Enter:
            gameName = gameLoader.GetSelectedGameName();
            gameSelected = true;
            break;
        case sk_Clear:
            gameSelected = true;
            break;
        case sk_Down:
            gameLoader.ChangeCursorIndex(1);
            break;
        case sk_Up:
            gameLoader.ChangeCursorIndex(-1);
            break;
        }
    }

    return gameName;
}

void ExecuteInstructions(Interpreter& interpreter, TimeManager& timeManager, float& amountOfInstructionsToExecute)
{
    while (amountOfInstructionsToExecute > 0){
        bool waitForVblank = interpreter.EmulateCycle();
        timeManager.IncrementCycleCounter();       
        amountOfInstructionsToExecute -= 1.0f;

        if (waitForVblank) return;
    }    
}

void UpdateTimers(Interpreter& interpreter, float& amountOfUpdates)
{
    while (amountOfUpdates > 0){
        interpreter.UpdateTimers();
        amountOfUpdates -= 1.0f;
    }
}

void TryRender(Interpreter& interpreter)
{
    auto& renderer = Renderer::GetInstance();
    if (interpreter.GetDrawFlag())
    {
        interpreter.SetDrawFlag(false);
        renderer.RenderScreen();
        renderer.SwapBuffer();
    }
}

void EndFrame(TimeManager& timeManager)
{
    timeManager.IncrementFrameCounter();
    timeManager.LimitFrameRate();
}