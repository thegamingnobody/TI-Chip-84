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

int main(void)
{
    gfx_Begin();

    auto& renderer = Renderer::GetInstance();
    // auto& interpreter = Interpreter::GetInstance();
    
    int const canvasX{64};
    int const canvasY{72};
    int const renderScale{3};
    
    renderer.Init(canvasX, canvasY, renderScale);

    // auto renderer = Renderer(canvasX, canvasY, renderScale);
    auto interpreter = Interpreter();
    
    //Todo: read in rom file
    // interpreter.LoadGame("LOGO");
    
    interpreter.RegisterAppName("LOGO");

    bool continueRunning{ true };
    EmulatorStates emulatorState{ EmulatorStates::Step };
    while (continueRunning)
    {
        if (os_GetCSC()) continueRunning = false;
        
        //Todo implement emulator states
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
            break;
        case EmulatorStates::Step:
            //Todo: execute 1 instruction
            interpreter.EmulateCycle();
            emulatorState = EmulatorStates::Paused;
            break;
        case EmulatorStates::Reset:
            break;
        case EmulatorStates::Loading_Game:
            break;
        }

        // RenderScreen();
        
        renderer.ResetBackground();

        renderer.SetPixel(2, 2, true);


        renderer.SwapBuffer();
    }

    gfx_End();

    return 0;
}