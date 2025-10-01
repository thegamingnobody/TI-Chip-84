#include "Renderer.h"

Renderer Renderer_Init(int canvasX, int canvasY, int renderScale)
{
    Renderer result;

    result.CanvasX = canvasX;
    result.CanvasY = canvasY;
    result.RenderScale = renderScale;

    return result;
}

void Renderer_ResetBackground(Renderer* renderer)
{
    gfx_SetColor(0x00);
    gfx_FillRectangle(renderer->CanvasX, renderer->CanvasY, 64 * renderer->RenderScale, 32 * renderer->RenderScale);

}

void Renderer_DrawPixel(Renderer* renderer, int x, int y)
{
    gfx_SetColor(0xFF);
    gfx_FillRectangle(renderer->CanvasX + (x * renderer->RenderScale), renderer->CanvasY + (y * renderer->RenderScale), renderer->RenderScale, renderer->RenderScale);
}