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
    
	m_Screen.resize(VIEWPORT_HEIGHT_BASE);
	for (int row = 0; row < VIEWPORT_HEIGHT_BASE; row++)
	{
		m_Screen[row].resize(VIEWPORT_WIDTH_BASE);
	}
}

void Renderer::RenderScreen()
{
    ResetBackground();

	for (int row = 0; row < VIEWPORT_HEIGHT_BASE; row++)
	{
		for (int col = 0; col < VIEWPORT_WIDTH_BASE; col++)
		{
			if (!m_Screen[row][col]) continue;

			RenderPixel(col, row);
		}
	}

}

void Renderer::ResetBackground()
{
    // gfx_FillScreen(m_White);
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
    m_Screen[y][x] = isEnabled;
}

bool Renderer::IsPixelOn(int x, int y) const
{
   	return m_Screen[y][x];
}

void Renderer::RenderPixel(int x, int y)
{
    gfx_SetColor(m_White);    
    gfx_FillRectangle(CanvasX + (x * RenderScale), CanvasY + (y * RenderScale), RenderScale, RenderScale);
}