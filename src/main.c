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

void DrawPixel(int x, int y);

int canvasX = 10;
int canvasY = 10;
int renderScale = 3;

int main(void)
{
    /* Initialize graphics drawing */
    gfx_Begin();

    /* draw background */
    gfx_SetColor(0x00);
    gfx_FillRectangle(canvasX, canvasY, 64 * renderScale, 32 * renderScale);

    /* draw a pixel */
    DrawPixel(1, 1);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfx_End();

    return 0;
}

void DrawPixel(int x, int y)
{
    gfx_SetColor(0xFF);
    gfx_FillRectangle(canvasX + (x * renderScale), canvasY + (y * renderScale), renderScale, renderScale);
}