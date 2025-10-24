#include "Renderer.h"
#include <graphx.h>


void Chip8::Renderer::Init(int canvasX, int canvasY, int renderScale)
{
    CanvasX = canvasX;
    CanvasY = canvasY;
    RenderScale = renderScale;
}

void Chip8::Renderer::ResetBackground()
{
    gfx_SetColor(0x00);
    gfx_FillRectangle(CanvasX, CanvasY, 64 * RenderScale, 32 * RenderScale);
}

void Chip8::Renderer::DrawPixel(int x, int y)
{
    gfx_SetColor(0xFF);
    gfx_FillRectangle(CanvasX + (x * RenderScale), CanvasY + (y * RenderScale), RenderScale, RenderScale);
}