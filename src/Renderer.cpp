#include "Renderer.h"
#include <graphx.h>

// Renderer::Renderer(int canvasX, int canvasY, int renderScale)
// {
//     Init(canvasX, canvasY, renderScale);
// }

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

void Renderer::TogglePixel(int x, int y)
{
    bool isPixelOn = IsPixelOn(x, y);
	SetPixel(x, y, !isPixelOn);
}

void Renderer::SwapBuffer()
{
    gfx_SwapDraw();
}

void Renderer::SetPixel(int x, int y, bool isEnabled)
{
    gfx_SetColor(isEnabled ? m_White : m_Black);    
    gfx_FillRectangle(CanvasX + (x * RenderScale), CanvasY + (y * RenderScale), RenderScale, RenderScale);
    // m_Screen[y][x] = isEnabled;
}

bool Renderer::IsPixelOn(int x, int y) const
{
   	// return m_Screen[y][x];
}