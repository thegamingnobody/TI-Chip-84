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

tinystl::string HandleGameChoice();

int main(void)
{
    int constexpr canvasX{64};
    int constexpr canvasY{32 + 40}; // 32 as screen height and 40 to center it vertically
    int constexpr renderScale{3};

    gfx_Begin();

    tinystl::string gameName = HandleGameChoice();

    gfx_SetColor(0xFF);    
    gfx_FillScreen(0xFF);
    
    auto& renderer = Renderer::GetInstance();
    auto& inputManager = InputManager::GetInstance();
    auto& timeManager = TimeManager::GetInstance();
    
    renderer.Init(canvasX, canvasY, renderScale);
    inputManager.Init();
    timeManager.Init();
    // auto renderer = Renderer(canvasX, canvasY, renderScale);
    auto interpreter = Interpreter();
      
    interpreter.LoadGame(gameName.c_str());

    timeManager.SetTimer();
    
    float instructionsToExecute{ 0.0f };
    float timerUpdates{ 0.0f };
    bool continueRunning{ true };
    while (continueRunning)
    {     
        continueRunning = inputManager.ProcessInput();
        
        timeManager.UpdateTime(false);
        float deltaTime = timeManager.GetDeltaTime();

        instructionsToExecute += deltaTime * timeManager.GetTargetIPF() * timeManager.GetTargetFPS(); //I per frame * FPS -> Istructions per second
        timerUpdates += deltaTime * timeManager.GetTargetFPS();

        while (instructionsToExecute >= 1.0f)
        {
            /*bool waitForVblank = */ interpreter.EmulateCycle();
            timeManager.IncrementCycleCounter();
            instructionsToExecute -= 1.0f;

            #ifdef DEBUG_PRINTS
            interpreter.PrintPC();
            #endif                
        }

        while (timerUpdates >= 1.0f)
        {
            interpreter.UpdateTimers();
            timerUpdates -= 1.0f;
        }
        
        if (interpreter.GetDrawFlag())
        {
            interpreter.ResetDrawFlag();
            renderer.RenderScreen();
            renderer.SwapBuffer();
        }
        
        timeManager.IncrementFrameCounter();

        timeManager.LimitFrameRate();
    }
   
    inputManager.Destroy();

    gfx_End();

    return 0;
}

tinystl::string HandleGameChoice()
{
    auto& gameLoader = GameLoader::GetInstance();
    
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