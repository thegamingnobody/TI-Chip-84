#pragma once

#include "Singleton.h"
#include <stdint.h>

class Renderer : public Singleton<Renderer>
{
public:
    void Init(int canvasX, int canvasY, int renderScale = 1.0f);

    void ResetBackground();
    void DrawPixel(int x, int y);

    void SwapBuffer();

private:
    //Todo: store pixels of screen? (tinystl vector?)

    int CanvasX;
    int CanvasY;
    int RenderScale;

    int const CanvasWidthBase{ 64 };
    int const CanvasHeightBase{ 32 };

    uint8_t const m_Black{ 0x00 };
    uint8_t const m_White{ 0xFF };
};