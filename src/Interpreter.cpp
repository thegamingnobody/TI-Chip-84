
#include "Interpreter.h"
#include <fileioc.h>
#include <graphx.h>
#include "Renderer.h"
#include <assert.h>
#include "sys/util.h"
#include <algorithm>
#include "InputManager.h"
#include "TypeDefinitions.h"
#include <debug.h>
#include "QuirkManager.h"

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
    uint16_t const opcode = FetchOpcode();

    // if not waiting for input
	if (!m_WaitForInput)
	{
		// timer.IncrementCycleCounter();
		m_PC += 2;
	}

    //Decode opcode
   	uint16_t const instructionType = (opcode & 0xF000) >> 8 >> 4;
	
	m_CurrentOpcode = opcode;

	switch (instructionType)
    {
    case 0x0:
        Instruction_0NNN(opcode);
        break; 
    case 0x1:
        Instruction_1NNN(opcode);
        break;  
    case 0x2:
        Instruction_2NNN(opcode);
        break;  
    case 0x3:
        Instruction_3XNN(opcode);
        break;  
    case 0x4:
        Instruction_4XNN(opcode);
        break;  
    case 0x5:
        Instruction_5XY0(opcode);
        break;  
    case 0x6:
        Instruction_6XNN(opcode);
        break;  
    case 0x7:
        Instruction_7XNN(opcode);
        break;  
    case 0x8:
        Instruction_8XYN(opcode);
        break;  
    case 0x9:
        Instruction_9XY0(opcode);
        break;  
    case 0xA:
        Instruction_ANNN(opcode);
        break;  
    case 0xB:
        Instruction_BNNN(opcode);
        break;  
    case 0xC:
        Instruction_CXNN(opcode);
        break;  
    case 0xD:
        Instruction_DXYN(opcode);
        break;  
    case 0xE:
        Instruction_EXNN(opcode);
        break;  
    case 0xF:
        Instruction_FXNN(opcode);
        break;  
    default:
        break;
    }

    return true;    
}

void Interpreter::UpdateTimers()
{
	if (m_DelayTimer > 0)
	{
		m_DelayTimer--;
	}
	if (m_SoundTimer > 0)
	{
		m_SoundTimer--;
	}
}

bool Interpreter::GetDrawFlag() const
{
	return m_DrawFlag;
}

void Interpreter::ResetDrawFlag()
{
	m_DrawFlag = false;
}

uint16_t Interpreter::FetchOpcode() const
{
    uint16_t const result = (m_Memory[m_PC] << 8) | m_Memory[m_PC+1];
    return result;
}

bool Interpreter::Instruction_0NNN(uint16_t baseInstruction)
{
	//0x00EE: Return from subroutine
	uint16_t const instructionParam = (baseInstruction & 0x0FFF);

	switch (instructionParam)
	{
	case 0x00E0:
		Renderer::GetInstance().FillScreen(false);
		m_DrawFlag = true;
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

bool Interpreter::Instruction_2NNN(uint16_t baseInstruction)
{
	//0x2NNN: Subroutine call. Jump to address NNN
	uint16_t const value = baseInstruction & 0xFFF;

	m_Stack.push(m_PC);

	m_PC = value;

	return true;
}

bool Interpreter::Instruction_3XNN(uint16_t baseInstruction)
{
	//0x3XNN: skip the next instruction if vX is equal to NN
	uint8_t const registerIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const value = (baseInstruction & 0x00FF);

	uint8_t const registerValue = m_V[registerIndex];

	if (registerValue == value)
	{
		m_PC += 2;
	}

	return true;
}

bool Interpreter::Instruction_4XNN(uint16_t baseInstruction)
{
	//0x4XNN: skip the next instruction if vX is NOT equal to NN
	uint8_t const registerIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const value = (baseInstruction & 0x00FF);

	uint8_t const registerValue = m_V[registerIndex];

	if (registerValue != value)
	{
		m_PC += 2;
	}

	return true;
}

bool Interpreter::Instruction_5XY0(uint16_t baseInstruction)
{
	//0x5XY0: skip the next instruction if vX is equal to vY
	uint8_t const registerXIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const registerYIndex = (baseInstruction & 0x00F0) >> 4;
	uint8_t const subInstruction = (baseInstruction & 0x000F);

	if (subInstruction != 0x0)
	{
		return false;
	}

	uint8_t const XValue = m_V[registerXIndex];
	uint8_t const YValue = m_V[registerYIndex];

	if (XValue == YValue)
	{
		m_PC += 2;
	}

	return true;
}

bool Interpreter::Instruction_6XNN(uint16_t baseInstruction)
{
	int const registerIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const value = baseInstruction & 0x00FF;
	m_V[registerIndex] = value;

	return true;
}

bool Interpreter::Instruction_7XNN(uint16_t baseInstruction)
{
	int const registerIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const value = baseInstruction & 0x00FF;
	m_V[registerIndex] = (m_V[registerIndex] + value) & 0xFF;

	return true;
}

bool Interpreter::Instruction_8XYN(uint16_t baseInstruction)
{
	//	 0x8XYN: various different instructions based on the value of N
	//			- 0x8XY0: vX is set to value of vY
	//			- 0x8XY1: vX is set to the result of a binary OR  between vX and vY, vY is not affected
	//			- 0x8XY2: vX is set to the result of a binary AND between vX and vY, vY is not affected
	//			- 0x8XY3: vX is set to the result of a binary XOR between vX and vY, vY is not affected
	//			- 0x8XY4: vX is set to vX + vY, vY is not affected. set vF to 1 if addition overflows, otherwise set vF to 0
	//			- 0x8XY5: vX is set to vX - vY, vY is not affected. if vX is larger, set vF to 1, otherwise set vF to 0
	//			- 0x8XY7: vX is set to vY - vX, vY is not affected. if vY is larger, set vF to 1, otherwise set vF to 0
	//			- 0x8XY6: 1) (configurable) vX is set to vY 
	//					  2) shift vX one bit to the right
	//					  3) set vF to to the value of the bit that was shifted out
	//			- 0x8XYE: 1) (configurable) vX is set to vY 
	//					  2) shift vY one bit to the left
	//					  3) set vF to to the value of the bit that was shifted out
	uint8_t const registerXIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const registerYIndex = (baseInstruction & 0x00F0) >> 4;
	uint8_t const subInstruction = (baseInstruction & 0x000F);

	uint8_t XValue = m_V[registerXIndex];
	uint8_t YValue = m_V[registerYIndex];

	bool const vFResetQuirk = !QuirkManager::GetInstance().GetVfResetQuirk();

	switch (subInstruction)
	{
	case 0x0:
		m_V[registerXIndex] = YValue;
		break;
	case 0x1:
		m_V[registerXIndex] = (XValue | YValue);
		if (vFResetQuirk)
		{
			m_V[0x0F] = 0;
		}
		break;
	case 0x2:
		m_V[registerXIndex] = (XValue & YValue);
		if (vFResetQuirk)
		{
			m_V[0x0F] = 0;
		}
		break;
	case 0x3:
		m_V[registerXIndex] = (XValue ^ YValue);
		if (vFResetQuirk)
		{
			m_V[0x0F] = 0;
		}
		break;
	case 0x4:
		{	
			uint16_t result = (XValue + YValue);
			m_V[registerXIndex] = (result & 0x00FF);
			if ((result & 0xFF00))
			{
				m_V[0xF] = 0x01;
			}
			else
			{
				m_V[0xF] = 0x00;
			}
		}
		break;
	case 0x5:
		{	
			uint16_t result = (XValue - YValue);
			m_V[registerXIndex] = (result & 0x00FF);
			if (result & 0xFF00)
			{
				m_V[0xF] = 0x00;
			}
			else
			{
				m_V[0xF] = 0x01;
			}
		}
		break;
	case 0x6:
		{
			bool shiftQuirk = QuirkManager::GetInstance().GetShiftQuirk();

			if (!shiftQuirk)
			{
				m_V[registerXIndex] = YValue;
				XValue = m_V[registerXIndex];
			}
			uint8_t lostBit = (XValue & 0x01);

			XValue = XValue >> 1;
			m_V[registerXIndex] = XValue;

			m_V[0xF] = lostBit;
		}
		break;
	case 0x7:
		{	
			uint16_t result = (YValue - XValue);
			m_V[registerXIndex] = (result & 0x00FF);
			if (result & 0xFF00)
			{
				m_V[0xF] = 0x00;
			}
			else
			{
				m_V[0xF] = 0x01;
			}
		}
		break;
	case 0xE:
		{
			bool shiftQuirk = QuirkManager::GetInstance().GetShiftQuirk();

			if (!shiftQuirk)
			{
				m_V[registerXIndex] = YValue;
				XValue = m_V[registerXIndex];
			}
			uint8_t lostBit = (XValue & 0x80);

			XValue = XValue << 1;
			m_V[registerXIndex] = XValue;

			m_V[0xF] = lostBit >> 7;
		}
		break;
	default:
		return false;
	}

	return true;
}

bool Interpreter::Instruction_9XY0(uint16_t baseInstruction)
{
	//0x9XY0: skip the next instruction if vX is NOT equal to vY
	uint8_t const registerXIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const registerYIndex = (baseInstruction & 0x00F0) >> 4;
	uint8_t const subInstruction = (baseInstruction & 0x000F);

	if (subInstruction != 0x0)
	{
		return false;
	}

	uint8_t const XValue = m_V[registerXIndex];
	uint8_t const YValue = m_V[registerYIndex];

	if (XValue != YValue)
	{
		m_PC += 2;
	}

	return true;
}

bool Interpreter::Instruction_ANNN(uint16_t baseInstruction)
{
	uint16_t const value = baseInstruction & 0x0FFF;
	m_I = value;

	return true;
}

bool Interpreter::Instruction_BNNN(uint16_t baseInstruction)
{
	uint16_t const jumpValue = (baseInstruction & 0x0FFF);
	uint8_t xIndex = 0;
	bool const jumpQuirk = QuirkManager::GetInstance().GetJumpQuirk();
	if (jumpQuirk)
	{
		xIndex = (baseInstruction & 0x0F00) >> 8;
	}

	uint8_t registerValue = m_V[xIndex];
	m_PC = jumpValue + registerValue;

	// 1) originally
	//0xBNNN: jump to address (NNN + value in v0)
	// 2) later on
	//0xBXNN: jump to address (XNN + value in vX)
	return true;
}

bool Interpreter::Instruction_CXNN(uint16_t baseInstruction)
{
	//0xCXNN: 1) generate random number
	//				2) binary AND random number with NN
	//				3) store result of binary AND in vX
	uint8_t const registerXIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const mask = (baseInstruction & 0x00FF);
	uint8_t const randomNumber{ static_cast<uint8_t>(random() & 0xFF) };

	uint8_t const result = randomNumber & mask;
	m_V[registerXIndex] = result;
	return true;
}

bool Interpreter::Instruction_DXYN(uint16_t baseInstruction)
{
	auto& renderer = Renderer::GetInstance();
	bool const wrapQuirk = QuirkManager::GetInstance().GetWrapQuirk();

	int const xIndex = (baseInstruction & 0x0F00) >> 8;
	int const yIndex = (baseInstruction & 0x00F0) >> 4;
	int const height = (baseInstruction & 0x000F);

	uint8_t const baseCoordX = m_V[xIndex] % VIEWPORT_WIDTH_BASE;
	uint8_t const baseCoordY = m_V[yIndex] % VIEWPORT_HEIGHT_BASE;
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
			else if (pixelY >= VIEWPORT_HEIGHT_BASE or pixelX >= VIEWPORT_WIDTH_BASE) continue;

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

	m_DrawFlag = true;

	return true;
}

bool Interpreter::Instruction_EXNN(uint16_t baseInstruction)
{
    // Todo: implement E instruction

	//0xEX9E: skip next instruction if key corresponding to value in vX is pressed
	//0xEXA1: skip next instruction if key corresponding to value in vX is NOT pressed
	//valid key values: 0 - F
	auto& inputManager = InputManager::GetInstance();

	uint8_t const registerXIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const subInstruction = (baseInstruction & 0x00FF);
	
	uint8_t const XValue = m_V[registerXIndex] & 0xF;

	switch (subInstruction)
	{
	case 0xA1:
		if (!inputManager.IsKeyPressed(XValue))
		{
			m_PC += 2;
		}
		break;
	case 0x9E:
		if (inputManager.IsKeyPressed(XValue))
		{
			m_PC += 2;
		}
		break;
	default:
		break;
	}

	return true;
}

bool Interpreter::Instruction_FXNN(uint16_t baseInstruction)
{
	auto& inputManager = InputManager::GetInstance();

	uint8_t const registerXIndex = (baseInstruction & 0x0F00) >> 8;
	uint8_t const subInstruction = (baseInstruction & 0x00FF);

	uint8_t const XValue = m_V[registerXIndex];

	switch (subInstruction)
	{
	case 0x07:
		//0xFX07: vX is set to current value of delay timer
		m_V[registerXIndex] = m_DelayTimer;
		break;
	case 0x15:
		//0xFX15: delay timer is set to vX
		m_DelayTimer = m_V[registerXIndex];
		break;
	case 0x18:
		//0xFX18: sound timer is set to vX
		m_SoundTimer = XValue;
		break;
	case 0x1E:
		//0xFX1E: index register is set to (index register + vX) => I += vX
		m_I += XValue;
		break;
	case 0x0A:
        //Todo: 

		// //0xFX0A: stops instruction execution untill a key is pressed. Decrement PC unless a key is pressed. delay and sound timers should still decrease. When key is pressed, its value is placed in vX and executions continues
		// //			on OG machine, key is registered upon release of key
		
		// //Check if already in wating mode
		if (!m_WaitForInput)
		{
			m_PC -= 2;
			m_WaitForInput = true;
			return true;
		}
		
		// //Check if any key is being pressed
		if (inputManager.isKeyReleasedThisFrame())
		{
			m_WaitForInput = false;
			for (int key = 0; key < inputManager.GetNumberOfKeys(); key++)
			{
				//set value of pressed key in vX
				//Todo: Set value of released key in vX
				if (inputManager.IsKeyReleased(key))
				{
					m_V[registerXIndex] = static_cast<uint8_t>(key & 0xFF);
					m_PC += 2;
					return true;
				}
			}
		}
		break;
	case 0x29:
		//0xFX29: set index register to address of character 
		m_I = FONTSET_ADDRESS + (CHARACTER_HEIGHT * XValue);
		break;
	case 0x33:
		//0xFX33: stores 3 digit decimal version of value in vX at address in index register.							example: if vX is 156, I will be 1, I + 1 will be 5 and I + 2 will be 6
		{
			uint8_t hundreds = XValue / 100;
			uint8_t tens = (XValue - hundreds * 100) / 10;
			uint8_t singleDigit = (XValue - hundreds * 100 - tens * 10);

			m_Memory[m_I + 0] = hundreds;
			m_Memory[m_I + 1] = tens;
			m_Memory[m_I + 2] = singleDigit;
		}
		break;
	case 0x55:
		{
			//0xFX55: store values of registers v0 to vX (inclusive) sequeltially starting at address in index register,	example: I will be v0, I + 1 will be v1, I + 2 will be v2 etc.
			bool loadStoreQuirkIncrement = QuirkManager::GetInstance().GetLoadStoreQuirkIncrement();
			bool loadStoreQuirkUnchanged = QuirkManager::GetInstance().GetLoadStoreQuirkUnchanged();

			for (int i = 0; i <= registerXIndex; i++)
			{
				if (loadStoreQuirkUnchanged)
				{
					m_Memory[m_I + i] = m_V[i];
				}
				else
				{
					m_Memory[m_I] = m_V[i];
					m_I++;
				}


			}
			if (!loadStoreQuirkUnchanged && loadStoreQuirkIncrement)
			{
				m_I--;
			}

		}
		break;
	case 0x65:
		{
			//0xFX65: reverse of 0xFX55, stores values from I to I + X in v0 t vX. congif: does I increment or does it use a temp value
			bool loadStoreQuirkIncrement = QuirkManager::GetInstance().GetLoadStoreQuirkIncrement();
			bool loadStoreQuirkUnchanged = QuirkManager::GetInstance().GetLoadStoreQuirkUnchanged();
		
			for (int i = 0; i <= registerXIndex; i++)
			{
				if (loadStoreQuirkUnchanged)
				{
					m_V[i] = m_Memory[m_I + i];
				}
				else
				{
					m_V[i] = m_Memory[m_I];
					m_I++;
				}
			}
			if (!loadStoreQuirkUnchanged && loadStoreQuirkIncrement)
			{
				m_I--;
			}
		}
		break;
	default:
		return false;
	}

	return true;
}