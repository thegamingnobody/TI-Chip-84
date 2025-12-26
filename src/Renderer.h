#pragma once

#include "Singleton.h"
#include "TypeDefinitions.h"
#include <TINYSTL/vector.h>

// #define canvas ((gfx_sprite_t*)m_Screen)

class Renderer : public Singleton<Renderer>
{
public:
    // Renderer(int canvasX, int canvasY, int renderScale = 1.0f);
    void Init(int canvasX, int canvasY, int renderScale = 1.0f);

    void RenderScreen();

    void ResetBackground();
	void TogglePixel(int x, int y);

	void SetPixel(int x, int y, bool isEnabled);
	bool IsPixelOn(int x, int y) const;

    void SwapBuffer();

    void FillScreen(bool fillValue);
private:
    void RenderPixel(int x, int y);


    int CanvasX;
    int CanvasY;
    int RenderScale;

    uint8_t const m_Black{ 0x00 };
    uint8_t const m_White{ 0xFF };

	tinystl::vector<tinystl::vector<uint8_t>> m_Screen;

    // uint8_t m_Screen[2048]{};

};