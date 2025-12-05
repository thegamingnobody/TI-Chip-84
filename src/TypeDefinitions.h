#pragma once
#include <stdint.h>

#define PROGRAM_COUNTER_START 0x0200
#define FONTSET_ADDRESS 0x50
#define CHARACTER_HEIGHT 5
#define MEMORY_SIZE 4096
#define NR_OF_REGISTERS 16
#define WINDOW_WIDTH_BASE 960
#define WINDOW_HEIGHT_BASE 540
#define VIEWPORT_WIDTH_BASE 64
#define VIEWPORT_HEIGHT_BASE 32
#define HEADER_SIZE 22

// #define DEBUG_PRINTS

enum class EmulatorStates
{
    Running,
    Paused,
    Step,
    Reset,
    Loading_Game
};