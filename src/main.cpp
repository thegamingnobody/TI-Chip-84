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
    
    renderer.Init(canvasX, canvasY, renderScale);
    inputManager.Init();
    // auto renderer = Renderer(canvasX, canvasY, renderScale);
    auto interpreter = Interpreter();
      
    interpreter.LoadGame(gameName.c_str());

    bool continueRunning{ true };
    // EmulatorStates emulatorState{ EmulatorStates::Paused };
    EmulatorStates emulatorState{ EmulatorStates::Running };
    while (continueRunning)
    {     
        continueRunning = inputManager.ProcessInput();
        
        switch (emulatorState)
        {
        case EmulatorStates::Running:
            //Todo: execute X amount of instructions 
            for (int i = 0; i < 11; i++)
            {
                /*bool waitForVblank = */ interpreter.EmulateCycle();
            }
            break;
        case EmulatorStates::Paused:
            break;
            
        case EmulatorStates::Step:
            interpreter.EmulateCycle();
            emulatorState = EmulatorStates::Paused;
            break;
            
        case EmulatorStates::Reset:
            break;
            
        case EmulatorStates::Loading_Game:
            break;
        }

        interpreter.UpdateTimers();
        
        #ifdef DEBUG_PRINTS
        interpreter.PrintPC();
        #endif

        renderer.RenderScreen();
        
        renderer.SwapBuffer();
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