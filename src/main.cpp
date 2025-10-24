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

// void AppvarDinges()
// {
//     char nameBuffer[10];
//     char string[] = "logo string";
//     uint8_t var;

//     // os_ClrHome();
    
//     /* Open a new variable; deleting it if it already exists */
//     var = ti_Open("LOGO", "r");

//     /* Ensure the name of the AppVar is correct */
//     ti_GetName(nameBuffer, var);

//     /* Ensure that the slot is closed */
//     char test[numbersToPrint];

//     // int headerSize = 22;
//     int headerSize = 0;


//     for(int i = 0; i < headerSize; i++)
//     {
//         ti_GetC(var);
//     }


//     for(int i = 0; i < numbersToPrint; i++)
//     {
//         test[i] = ti_GetC(var);
//         sprintf(test, "%02X", (unsigned char)test[i]);
//         // gfx_PrintStringXY(test,
//         //                 (18 * i),
//         //                 1);
//         gfx_PrintStringXY(test,
//                         30,
//                         10);

//     }

//     ti_Close(var);
//     var = 0;

// }

void RenderScreen()
{
    auto& renderer = Renderer::GetInstance();

    renderer.ResetBackground();

    renderer.DrawPixel(2, 2);
}

int main(void)
{
   	auto& renderer = Renderer::GetInstance();
    
    int const canvasX{64};
    int const canvasY{72};
    int const renderScale{3};

    renderer.Init(canvasX, canvasY, renderScale);

    /* Initialize graphics drawing */
    gfx_Begin();

    // AppvarDinges();

    RenderScreen();

    while (!os_GetCSC());

    /* End graphics drawing */
    gfx_End();

    return 0;
}