#include "Renderer.h"
#include <debug.h>

// Renderer::Renderer(int canvasX, int canvasY, int renderScale)
// {
//     Init(canvasX, canvasY, renderScale);
// }

void Renderer::Init(int canvasX, int canvasY, int renderScale)
{
    CanvasX = canvasX;
    CanvasY = canvasY;
    RenderScale = renderScale;
	
	m_Screen = gfx_MallocSprite(64, 32);
	FillScreen(false);
}

void Renderer::Destroy()
{
}

void Renderer::RenderScreen()
{
	gfx_ScaledSprite_NoClip(m_Screen, CanvasX, CanvasY, RenderScale, RenderScale);
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
    m_Screen->data[(y * VIEWPORT_WIDTH_BASE) + x] = (isEnabled ? m_White : m_Black);
}

bool Renderer::IsPixelOn(int x, int y) const
{
   	return m_Screen->data[(y * VIEWPORT_WIDTH_BASE) + x];
}


void Renderer::FillScreen(bool fillValue)
{
	for (int pixel = 0; pixel < VIEWPORT_HEIGHT_BASE * VIEWPORT_WIDTH_BASE; pixel++)
	{
		m_Screen->data[pixel] = (fillValue ? m_White : m_Black);
	}
}