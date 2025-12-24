#pragma once

#include "Singleton.h"
#include "TypeDefinitions.h"
#include <TINYSTL/vector.h>

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

    int const CanvasWidthBase{ 64 };
    int const CanvasHeightBase{ 32 };

    uint8_t const m_Black{ 0x00 };
    uint8_t const m_White{ 0xFF };

    //TODO: do I need this? In the original I stored this to be able to render to the texture, but now I don't do that anymore
	tinystl::vector<tinystl::vector<bool>> m_Screen;
};