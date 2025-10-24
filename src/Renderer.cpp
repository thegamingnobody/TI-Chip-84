#include "Renderer.h"
#include <graphx.h>

void Renderer::Init(int canvasX, int canvasY, int renderScale)
{
    CanvasX = canvasX;
    CanvasY = canvasY;
    RenderScale = renderScale;
}

void Renderer::ResetBackground()
{
    gfx_SetColor(m_Black);
    gfx_FillRectangle(CanvasX, CanvasY, CanvasWidthBase * RenderScale, CanvasHeightBase * RenderScale);
}

void Renderer::DrawPixel(int x, int y)
{
    gfx_SetColor(m_White);
    gfx_FillRectangle(CanvasX + (x * RenderScale), CanvasY + (y * RenderScale), RenderScale, RenderScale);

}