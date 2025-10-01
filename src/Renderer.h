#pragma once


//typedef X Y where X is the struct and Y is the new name
typedef struct Renderer
{
    int CanvasX;
    int CanvasY;
    int RenderScale;
} Renderer;

Renderer Renderer_Init(int canvasX, int canvasY, int renderScale);

//Resets the game background to black
void Renderer_ResetBackground(Renderer* renderer);

//Draw a single pixel of the game
void Renderer_DrawPixel(Renderer* renderer, int x, int y);
