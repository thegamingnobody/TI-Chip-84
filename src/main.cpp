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
    // auto& timeManager = TimeManager::GetInstance();
    auto timeManager = TimeManager();
    
    renderer.Init(canvasX, canvasY, renderScale);
    inputManager.Init();
    timeManager.Init();
    quirkManager.Init();
    auto interpreter = Interpreter();
      
    interpreter.LoadGame(gameName.c_str());

    timeManager.SetTimer();
    
    float instructionsToExecute{ 0.0f };
    float timerUpdates{ 0.0f };
    bool continueRunning{ true };
    while (continueRunning)
    {     
        timeManager.StartFrame();

        // Check for draw request
        if (interpreter.GetRequestDrawFlag())
        {
            interpreter.SetRequestDrawFlag(false);
            interpreter.SetDrawFlag(true);
        }

        continueRunning = inputManager.ProcessInput();
        
        // Update time and calculate how many instructions to execute
        timeManager.UpdateTime(false);
        float deltaTime = timeManager.GetDeltaTime();
        
        instructionsToExecute += deltaTime * timeManager.GetTargetIPF() * timeManager.GetTargetFPS(); //I per frame * FPS -> Istructions per second
        timerUpdates += deltaTime * timeManager.GetTargetFPS();
        
        // Execute instructions
        while (instructionsToExecute >= 1.0f)
        {
            bool waitForVblank = interpreter.EmulateCycle();
            timeManager.IncrementCycleCounter();
            instructionsToExecute -= 1.0f;       
            if (waitForVblank)
            {
                instructionsToExecute -= static_cast<int>(instructionsToExecute); //will leave only the part after the decimal
            }     
        }
        
        // Update timers
        while (timerUpdates >= 1.0f)
        {
            interpreter.UpdateTimers();
            timerUpdates -= 1.0f;
        }
        
        // Render if draw flag is set
        if (interpreter.GetDrawFlag())
        {
            interpreter.SetDrawFlag(false);
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