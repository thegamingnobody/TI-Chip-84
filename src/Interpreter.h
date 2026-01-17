#pragma once

#include "TypeDefinitions.h"
#include <TINYSTL/vector.h>
#include <TINYSTL/string.h>
#include "Stack.h"


class Interpreter
{
public:
    Interpreter();
	void Init();

    void LoadGame(const char* appVarName);

	bool EmulateCycle();

	void UpdateTimers();

	bool GetDrawFlag() const;
	void SetDrawFlag(bool newValue);

private:
	uint16_t FetchOpcode() const;

	void LoadFontSet();

	//Instruction functions
	bool Instruction_0NNN(uint16_t baseInstruction);
	bool Instruction_1NNN(uint16_t baseInstruction);
	bool Instruction_2NNN(uint16_t baseInstruction);
	bool Instruction_3XNN(uint16_t baseInstruction);
	bool Instruction_4XNN(uint16_t baseInstruction);
	bool Instruction_5XY0(uint16_t baseInstruction);
	bool Instruction_6XNN(uint16_t baseInstruction);
	bool Instruction_7XNN(uint16_t baseInstruction);
	bool Instruction_8XYN(uint16_t baseInstruction);
	bool Instruction_9XY0(uint16_t baseInstruction);
	bool Instruction_ANNN(uint16_t baseInstruction);
	bool Instruction_BNNN(uint16_t baseInstruction);
	bool Instruction_CXNN(uint16_t baseInstruction);
	bool Instruction_DXYN(uint16_t baseInstruction);
	bool Instruction_EXNN(uint16_t baseInstruction);
	bool Instruction_FXNN(uint16_t baseInstruction);

	tinystl::string m_AppVarName;

	//Main memory
	tinystl::vector<uint8_t> m_Memory{};

	//Registers
	tinystl::vector<uint8_t> m_V{};

	//Stack 
	Stack m_Stack{};

	// //Index register
	uint16_t m_I{};

	//Program counter
	uint16_t m_PC{ PROGRAM_COUNTER_START };

	// //Timers & Stack Pointer
	uint8_t m_DelayTimer{};
	uint8_t m_SoundTimer{};
	// uint8_t m_SP{};

	bool m_DrawFlag{ true };
	bool m_WaitForInput{ false };

	uint16_t m_CurrentOpcode{};
};