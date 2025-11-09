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

int main(void)
{
    gfx_Begin();
    
    auto& gameLoader = GameLoader::GetInstance();
    
    gameLoader.ListChip8Games();
    
    //Todo: add game selection

    tinystl::string gameName;
    bool gameSelected{false};
    while(os_GetCSC() != sk_Clear && !gameSelected)
    {
        if (os_GetCSC() == sk_Enter)
        {
            gameName = gameLoader.GetSelectedGameName();
            gameSelected = true;
        }
        else if (os_GetCSC() == sk_Down)
        {
            gameLoader.ChangeCursorIndex(1);
        }
        else if (os_GetCSC() == sk_Up)
        {
            gameLoader.ChangeCursorIndex(-1);
        }
        
        
    }

    gfx_SetColor(0xFF);    
    gfx_FillScreen(0xFF);

    auto& renderer = Renderer::GetInstance();
    // auto& interpreter = Interpreter::GetInstance();
    
    int constexpr canvasX{64};
    int constexpr canvasY{32 + 40}; // 32 as screen height and 40 to center it vertically
    int constexpr renderScale{3};
    
    renderer.Init(canvasX, canvasY, renderScale);

    // auto renderer = Renderer(canvasX, canvasY, renderScale);
    auto interpreter = Interpreter();
      
    interpreter.LoadGame(gameName.c_str());

    // interpreter.LoadGame("LOGO");
    // interpreter.LoadGame("CORAX");
    // interpreter.LoadGame("FLAGS");
    // interpreter.LoadGame("QUIRKS");

    bool continueRunning{ true };
    EmulatorStates emulatorState{ EmulatorStates::Running };
    while (continueRunning)
    {
        if (os_GetCSC() == sk_Clear) 
        {
            continueRunning = false;
        }
        else if (os_GetCSC() == sk_Enter)
        {
            emulatorState = EmulatorStates::Step;

            while (os_GetCSC() == sk_Enter);
        }
        
        switch (emulatorState)
        {
            case EmulatorStates::Running:
                //Todo: execute X amount of instructions 
                for (int i = 0; i < 11; i++)
                {
                    bool waitForVblank = interpreter.EmulateCycle();
                }
                break;
            case EmulatorStates::Paused:
                // gfx_PrintStringXY("Pause", 5, 0);    
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
        
        // RenderScreen();
        
        // interpreter.PrintPC();
        
        renderer.RenderScreen();

        renderer.SwapBuffer();
    }

    gfx_End();

    return 0;
}