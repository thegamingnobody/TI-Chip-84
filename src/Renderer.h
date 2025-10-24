#pragma once

#include "Singleton.h"

namespace Chip8
{
    class Renderer : public Singleton<Renderer>
    {
    public:
        void Init(int canvasX, int canvasY, int renderScale = 1.0f);

        void ResetBackground();
        void DrawPixel(int x, int y);

    private:
        int CanvasX;
        int CanvasY;
        int RenderScale;
    };
}