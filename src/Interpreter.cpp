
#include "Interpreter.h"
#include <fileioc.h>
#include <graphx.h>
#include "Renderer.h"
#include <assert.h>

Interpreter::Interpreter()
{
    Init();
}

void Interpreter::Init()
{
   	m_Memory.resize(MEMORY_SIZE);
    m_V.resize(NR_OF_REGISTERS);
}

void Interpreter::LoadGame(const char* appVarName)
{
    m_AppVarName = appVarName;

    uint8_t handle;

    handle = ti_OpenVar(m_AppVarName.c_str(), "r", OS_TYPE_APPVAR);
    if (!handle) 
    {
        // gfx_PrintStringXY("AppVar not found", 0, 0);
        return;
    }

    uint16_t size = ti_GetSize(handle);

    ti_Seek(HEADER_SIZE, SEEK_SET, handle);

    // Calculate how many bytes to load (don’t overflow memory)
    uint16_t programSize = size - HEADER_SIZE;
    if (PROGRAM_COUNTER_START + programSize > MEMORY_SIZE)
    {
        programSize = MEMORY_SIZE - PROGRAM_COUNTER_START;
    }

    ti_Seek(HEADER_SIZE, SEEK_SET, handle);
    ti_Read(m_Memory.data() + PROGRAM_COUNTER_START, 1, programSize, handle);
    ti_Close(handle);   

    m_PC = PROGRAM_COUNTER_START;
}

bool Interpreter::EmulateCycle()
{
    //Fetch opcode
    uint16_t opcode = FetchOpcode();

    // if not waiting for input
	// timer.IncrementCycleCounter();
	m_PC += 2;

    // gfx_PrintInt(m_PC, 2);

    //Decode opcode
   	uint8_t instructionType = (opcode & 0xF000) >> 12;

    switch (instructionType)
    {
    case 0x0:
        Instruction_0NNN(opcode);
        break; 
    case 0x1:
        Instruction_1NNN(opcode);
        break;  
    case 0x2:
        // Instruction_2NNN(opcode);
        break;  
    case 0x3:
        // Instruction_3XNN(opcode);
        break;  
    case 0x4:
        // Instruction_4XNN(opcode);
        break;  
    case 0x5:
        // Instruction_5XY0(opcode);
        break;  
    case 0x6:
        Instruction_6XNN(opcode);
        break;  
    case 0x7:
        Instruction_7XNN(opcode);
        break;  
    case 0x8:
        // Instruction_8XYN(opcode);
        break;  
    case 0x9:
        // Instruction_9XY0(opcode);
        break;  
    case 0xA:
        Instruction_ANNN(opcode);
        break;  
    case 0xB:
        // Instruction_BNNN(opcode);
        break;  
    case 0xC:
        // Instruction_DXYN(opcode);
        break;  
    case 0xD:
        Instruction_DXYN(opcode);
        break;  
    case 0xE:
        // Instruction_EXNN(opcode);
        break;  
    case 0xF:
        // Instruction_FXNN(opcode);
        break;  
    default:
        break;
    }

    return true;    
}

void Interpreter::PrintPC() const
{
    gfx_FillScreen(0xFF);
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(m_PC, 4);
}

uint16_t Interpreter::FetchOpcode()
{
    uint16_t result = (m_Memory[m_PC] << 8) | m_Memory[m_PC+1];
    return result;
}

bool Interpreter::Instruction_0NNN(uint16_t baseInstruction)
{
	//0x00EE: Return from subroutine
	uint16_t instructionParam = (baseInstruction & 0x0FFF);

	switch (instructionParam)
	{
	case 0x00E0:
		Renderer::GetInstance().ResetBackground();
		// m_DrawFlag = true;
		break;
	case 0x00EE:
		m_PC = m_Stack.top();
		m_Stack.pop();
		break;
	default:
		return false;
	}

	return true;
}

bool Interpreter::Instruction_1NNN(uint16_t baseInstruction)
{
	m_PC = baseInstruction & 0x0FFF;
	return true;
}

// bool Interpreter::Instruction_2NNN(uint16_t baseInstruction)
// {
// 	//0x2NNN: Subroutine call. Jump to address NNN
// 	uint16_t value = baseInstruction & 0xFFF;
//
// 	m_Stack.push(m_PC);
//
// 	m_PC = value;
//
// 	return true;
// }

// bool Interpreter::Instruction_3XNN(uint16_t baseInstruction)
// {
// 	//0x3XNN: skip the next instruction if vX is equal to NN
// 	uint8_t registerIndex = (baseInstruction & 0x0F00) >> 8;
// 	uint8_t value = (baseInstruction & 0x00FF);
//
// 	assert(registerIndex <= 0xF);
//
// 	uint8_t registerValue = m_V[registerIndex];
//
// 	if (registerValue == value)
// 	{
// 		m_PC += 2;
// 	}
//
// 	return true;
// }

// bool Interpreter::Instruction_4XNN(uint16_t baseInstruction)
// {
// 	//0x4XNN: skip the next instruction if vX is NOT equal to NN
// 	uint8_t registerIndex = (baseInstruction & 0x0F00) >> 8;
// 	uint8_t value = (baseInstruction & 0x00FF);
//
// 	assert(registerIndex <= 0xF);
//
// 	uint8_t registerValue = m_V[registerIndex];
//
// 	if (registerValue != value)
// 	{
// 		m_PC += 2;
// 	}
//
// 	return true;
// }

// bool Interpreter::Instruction_5XY0(uint16_t baseInstruction)
// {
// 	//0x5XY0: skip the next instruction if vX is equal to vY
// 	uint8_t registerXIndex = (baseInstruction & 0x0F00) >> 8;
// 	uint8_t registerYIndex = (baseInstruction & 0x00F0) >> 4;
// 	uint8_t subInstruction = (baseInstruction & 0x000F);
//
// 	assert(registerXIndex <= 0xF);
// 	assert(registerYIndex <= 0xF);
//
// 	if (subInstruction != 0x0)
// 	{
// 		return false;
// 	}
//
// 	uint8_t XValue = m_V[registerXIndex];
// 	uint8_t YValue = m_V[registerYIndex];
//
// 	if (XValue == YValue)
// 	{
// 		m_PC += 2;
// 	}
//
// 	return true;
// }

bool Interpreter::Instruction_6XNN(uint16_t baseInstruction)
{
	int registerIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t value = baseInstruction & 0x00FF;
	m_V[registerIndex] = value;

	return true;
}

bool Interpreter::Instruction_7XNN(uint16_t baseInstruction)
{
	int registerIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t value = baseInstruction & 0x00FF;
	m_V[registerIndex] += value;

	return true;
}

bool Interpreter::Instruction_ANNN(uint16_t baseInstruction)
{
	uint16_t value = baseInstruction & 0x0FFF;
	m_I = value;

	return true;
}

bool Interpreter::Instruction_DXYN(uint16_t baseInstruction)
{
	auto& renderer = Renderer::GetInstance();
	// bool wrapQuirk = Chip8::QuirkManager::GetInstance().GetWrapQuirk();
	bool wrapQuirk = true;

	int xIndex = (baseInstruction & 0x0F00) >> 8;
	int yIndex = (baseInstruction & 0x00F0) >> 4;
	int height = (baseInstruction & 0x000F);

	uint8_t baseCoordX = m_V[xIndex] % VIEWPORT_WIDTH_BASE;
	uint8_t baseCoordY = m_V[yIndex] % VIEWPORT_HEIGHT_BASE;
	m_V[0xF] = 0;

	for (int row = 0; row < height; row++)
	{
		auto pixelY = (baseCoordY + row);

		if (wrapQuirk)
		{
			pixelY = pixelY % VIEWPORT_HEIGHT_BASE;
		}

		uint8_t spriteRow = m_Memory[m_I + row];

		for (int pixel = 0; pixel < 8; pixel++)
		{
			auto pixelX = (baseCoordX + pixel);

			if (wrapQuirk)
			{
				pixelX = pixelX % VIEWPORT_WIDTH_BASE;
			}
			else if (pixelY >= VIEWPORT_HEIGHT_BASE || pixelX >= VIEWPORT_WIDTH_BASE)
				continue;

			if (spriteRow & (0x80 >> pixel))
			{
				if (renderer.IsPixelOn(pixelX, pixelY))
				{
					m_V[0xF] = 1;
				}
				renderer.TogglePixel(pixelX, pixelY);
			}
		}
	}

	// m_DrawFlag = true;

	return true;
}
