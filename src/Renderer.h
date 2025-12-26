#pragma once

#include "Singleton.h"
#include "TypeDefinitions.h"
#include <TINYSTL/vector.h>
#include <graphx.h>

class Renderer : public Singleton<Renderer>
{
public:
    // Renderer(int canvasX, int canvasY, int renderScale = 1.0f);
    void Init(int canvasX, int canvasY, int renderScale = 1.0f);

    void RenderScreen();

	void TogglePixel(int x, int y);

	void SetPixel(int x, int y, bool isEnabled);
	bool IsPixelOn(int x, int y) const;

    void SwapBuffer();

    void FillScreen(bool fillValue);
private:
    int CanvasX;
    int CanvasY;
    int RenderScale;

    uint8_t const m_Black{ 0x00 };
    uint8_t const m_White{ 0xFF };

    gfx_sprite_t* m_Screen;

};