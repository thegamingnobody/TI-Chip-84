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

int main(void)
{
    /* Initialize graphics drawing */
    gfx_Begin();

    Renderer renderer = Renderer_Init(10, 10, 3);

    /* draw background */
    Renderer_ResetBackground(&renderer);

    /* draw a pixel */
    Renderer_DrawPixel(&renderer, 1, 10);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfx_End();

    return 0;
}