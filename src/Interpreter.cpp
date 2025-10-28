
#include "Interpreter.h"
#include <fileioc.h>
#include <graphx.h>
#include "Renderer.h"

Interpreter::Interpreter()
{
    Init();
}

void Interpreter::Init()
{

}

void Interpreter::RegisterAppName(const char* appVarName)
{
    m_AppVarName = appVarName;
}

bool Interpreter::EmulateCycle()
{
    uint16_t opcode = FetchOpcode();

    char test[5];

    sprintf(test, "%02X", (unsigned char)opcode);
    gfx_PrintString(test);
    sprintf(test, "%02X", (unsigned char)(opcode >> 8));
    gfx_PrintString(test);

    return true;    
}

uint16_t Interpreter::FetchOpcode()
{
    uint8_t handle;
    uint16_t result;

    // Open AppVar for reading
    handle = ti_OpenVar(m_AppVarName.c_str(), "r", OS_TYPE_APPVAR);
    if (!handle) 
    {
        gfx_PrintStringXY("AppVar not found", 0, 0);
        return 0;
    }

    // Skip header bytes if needed (most AppVars have a small header)
    ti_Seek(22, SEEK_SET, handle);

    ti_Read(&result, sizeof(uint16_t), 1, handle);
    
    ti_Close(handle);

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
