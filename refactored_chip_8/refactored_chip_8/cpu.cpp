#include "cpu.h"
#include "bus.h"

namespace helpers {
	inline uint16_t getFirstNibble(uint16_t inAddr)
	{
		return (inAddr & 0xF000) >> 12;
	}

	inline uint16_t getSecondNibble(uint16_t inAddr)
	{
		return (inAddr & 0x0F00) >> 8;
	}

	inline uint16_t getThirdNibble(uint16_t inAddr)
	{
		return (inAddr & 0x00F0) >> 4;
	}

	inline uint16_t getFourthNibble(uint16_t inAddr)
	{
		return inAddr & 0x000F;
	}

	inline uint16_t getSecondByte(uint16_t inAddr)
	{
		return inAddr & 0x00FF;
	}

	inline uint16_t getLastThreeNibbles(uint16_t inAddr)
	{
		return inAddr & 0x0FFF;
	}
}

cpu::cpu(bus* in_bus_link) : PC{ 0x200 }, SP{ 0xFF }, delayTimer{ 0 }, soundTimer{ 0 }, bus_link{ in_bus_link }, instruction{ 0 }
{
	srand(time(NULL));
}

cpu::~cpu()
{
}

void cpu::reset()
{
	PC = 0x200;
	SP = 0xFF;
	delayTimer = 0;
	soundTimer = 0;
	instruction = 0;
	memset(registers, 0, 16); // reset registers
	memset(stack, 0, 16*2); // reset stack
	I = 0;
}


#include <bitset>

void cpu::run()
{
	instruction = ((uint16_t)bus_link->memory.read(PC++) << 8) | ((uint16_t)bus_link->memory.read(PC++));

	uint16_t id_1 = helpers::getFirstNibble(instruction), id_2 = helpers::getFourthNibble(instruction), id_3 = helpers::getThirdNibble(instruction);

#ifdef chip_debug
	//std::cout << "PC: " << PC << std::endl;
	//std::cout << "instruction: " << std::hex << instruction << std::endl;
#endif

	uint16_t firstN = helpers::getFirstNibble(instruction), thirdN = helpers::getThirdNibble(instruction), fourthN = helpers::getFourthNibble(instruction), lastByte = helpers::getSecondByte(instruction);

	switch (firstN)
	{
	case 0:
		switch (instruction)
		{
		case 0x00E0:
			CLS();
			break;
		case 0x00EE:
			RET();
			break;
		default:
			SYS();
			break;
		}
		break;
	case 1:
		JP_1();
		break;
	case 2:
		CALL();
		break;
	case 3:
		SE_1();
		break;
	case 4:
		SNE_1();
		break;
	case 5:
		SE_2();
		break;
	case 6:
		LD_1();
		break;
	case 7:
		ADD_1();
		break;
	case 8:
		switch (fourthN)
		{
		case 0:
			LD_2();
			break;
		case 1:
			OR();
			break;
		case 2:
			AND();
			break;
		case 3:
			XOR();
			break;
		case 4:
			ADD_2();
			break;
		case 5:
			SUB();
			break;
		case 6:
			SHR();
			break;
		case 7:
			SUBN();
			break;
		case 0xE:
			SHL();
			break;
		}
		break;
	case 9:
		SNE_2();
		break;
	case 0xA:
		LD_3();
		break;
	case 0xB:
		JP_2();
		break;
	case 0xC:
		RND();
		break;
	case 0xD:
		DRW();
		break;
	case 0xE:
		switch (fourthN)
		{
		case 0xE:
			SKP();
			break;
		case 0x1:
			SKNP();
			break;
		}
		break;
	case 0xF:
		switch (lastByte)
		{
		case 0x07:
			LD_4();
			break;
		case 0x0A:
			LD_5();
			break;
		case 0x15:
			LD_6();
			break;
		case 0x18:
			LD_7();
			break;
		case 0x1E:
			ADD_3();
			break;
		case 0x29:
			LD_8();
			break;
		case 0x33:
			LD_9();
			break;
		case 0x55:
			LD_10();
			break;
		case 0x65:
			LD_11();
			break;
		}
		break;
	}
}

uint16_t cpu::getPC()
{
	return PC;
}

uint8_t cpu::getSP()
{
	return SP;
}

uint8_t* cpu::getRegister()
{
	return registers;
}

std::string cpu::opStr_instruction(uint16_t in_instruction)
{
	std::string op_str;

	uint16_t firstN = helpers::getFirstNibble(in_instruction), thirdN = helpers::getThirdNibble(in_instruction), fourthN = helpers::getFourthNibble(in_instruction), lastByte = helpers::getSecondByte(in_instruction);

	switch (firstN)
	{
	case 0:
		switch (instruction)
		{
		case 0x00E0:
			op_str = "CLS()";
			CLS();
			break;
		case 0x00EE:
			op_str = "RET()";
			RET();
			break;
		default:
			op_str = "SYS()";
			SYS();
			break;
		}
		break;
	case 1:
		op_str = "JP_1()";
		JP_1();
		break;
	case 2:
		op_str = "CALL()";
		CALL();				
		break;				
	case 3:					
		op_str = "SE_1()";
		SE_1();			
		break;			
	case 4:				
		op_str = "SNE_1()";
		SNE_1();			
		break;				
	case 5:					
		op_str = "SE_2()";
		SE_2();				
		break;				
	case 6:					
		op_str = "LD_1()";
		LD_1();				
		break;				
	case 7:					
		op_str = "ADD_1()";
		ADD_1();
		break;
	case 8:
		switch (fourthN)
		{
		case 0:
			op_str = "LD_2()";
			LD_2();
			break;
		case 1:
			op_str = "OR()";
			OR();
			break;
		case 2:
			op_str = "AND()";
			AND();
			break;
		case 3:
			op_str = "XOR()";
			XOR();
			break;
		case 4:
			op_str = "ADD_2()";
			ADD_2();
			break;
		case 5:
			op_str = "SUB()";
			SUB();
			break;
		case 6:
			op_str = "SHR()";
			SHR();
			break;
		case 7:
			op_str = "SUBN()";
			SUBN();
			break;
		case 0xE:
			op_str = "SHL()";
			SHL();
			break;
		}
		break;
	case 9:
		op_str = "SNE_2()";
		SNE_2();
		break;
	case 0xA:
		op_str = "LD_3()";
		LD_3();
		break;
	case 0xB:
		op_str = "JP_2()";
		JP_2();
		break;
	case 0xC:
		op_str = "RND()";
		RND();
		break;
	case 0xD:
		op_str = "DRW()";
		DRW();
		break;
	case 0xE:
		switch (fourthN)
		{
		case 0xE:
			std::cout << "SKP()";
			SKP();
			break;
		case 0x1:
			std::cout << "SKNP()";
			SKNP();
			break;
		}
		break;
	case 0xF:
		switch (lastByte)
		{
		case 0x07:
			op_str = "LD_4()";
			LD_4();
			break;
		case 0x0A:
			op_str = "LD_5()";
			LD_5();
			break;
		case 0x15:
			op_str = "LD_6()";
			LD_6();
			break;
		case 0x18:
			op_str = "LD_7()";
			LD_7();
			break;
		case 0x1E:
			op_str = "ADD_3()";
			ADD_3();
			break;
		case 0x29:
			op_str = "LD_8()";
			LD_8();
			break;
		case 0x33:
			op_str = "LD_9()";
			LD_9();
			break;
		case 0x55:
			op_str = "LD_10()";
			LD_10();
			break;
		case 0x65:
			op_str = "LD_11()";
			LD_11();
			break;
		}
		break;
	}
	return std::string();
}

/*
* INSTRUCTIONS
*/
void cpu::SYS()
{
	PC = helpers::getLastThreeNibbles(instruction);
}

void cpu::CLS()
{
	for (auto& row : bus_link->display)
		for (auto& cell : row)
			cell = false;
}

void cpu::RET()
{
	PC = stack[SP--];
}

void cpu::JP_1()
{
	PC = helpers::getLastThreeNibbles(instruction);
}

void cpu::CALL()
{
	stack[++SP] = PC;
	PC = helpers::getLastThreeNibbles(instruction);
}

void cpu::SE_1()
{
	if (registers[helpers::getSecondNibble(instruction)] == helpers::getSecondByte(instruction)) { PC += 2; }
}

void cpu::SNE_1()
{
	if (registers[helpers::getSecondNibble(instruction)] != helpers::getSecondByte(instruction)) { PC += 2; }
}

void cpu::SE_2()
{
	if (registers[helpers::getSecondNibble(instruction)] == registers[helpers::getThirdNibble(instruction)]) { PC += 2; }
}

void cpu::LD_1()
{
	registers[helpers::getSecondNibble(instruction)] = helpers::getSecondByte(instruction);
}

void cpu::ADD_1()
{
	registers[helpers::getSecondNibble(instruction)] += helpers::getSecondByte(instruction);
}

void cpu::LD_2()
{
	registers[helpers::getSecondNibble(instruction)] = registers[helpers::getThirdNibble(instruction)];
}

void cpu::OR()
{
	registers[helpers::getSecondNibble(instruction)] = registers[helpers::getSecondNibble(instruction)] | registers[helpers::getThirdNibble(instruction)];
}

void cpu::AND()
{
	registers[helpers::getSecondNibble(instruction)] = registers[helpers::getSecondNibble(instruction)] & registers[helpers::getThirdNibble(instruction)];
}

void cpu::XOR()
{
	registers[helpers::getSecondNibble(instruction)] = registers[helpers::getSecondNibble(instruction)] ^ registers[helpers::getThirdNibble(instruction)];
}

void cpu::ADD_2()
{
	uint16_t result = registers[helpers::getSecondNibble(instruction)] + registers[helpers::getThirdNibble(instruction)];

	if (result > 0xFF) { registers[0xF] = 1; }
	else { registers[0xF] = 0; }

	registers[helpers::getSecondNibble(instruction)] = (uint8_t)result & 0x00FF;
}

void cpu::SUB()
{
	if (registers[helpers::getSecondNibble(instruction)] > registers[helpers::getThirdNibble(instruction)]) { registers[0xF] = 1; }
	else { registers[0xF] = 0; }

	registers[helpers::getSecondNibble(instruction)] -= registers[helpers::getThirdNibble(instruction)];
}

void cpu::SHR()
{
	if ((instruction & 0x0001) > 0) { registers[0xF] = 1; } // If least significant bit is 1
	else { registers[0xF] = 0; }

	registers[helpers::getSecondNibble(instruction)] /= 2;
}

void cpu::SUBN()
{
	if (registers[helpers::getThirdNibble(instruction)] > registers[helpers::getSecondNibble(instruction)]) { registers[0xF] = 1; }
	else { registers[0xF] = 0; }

	registers[helpers::getSecondNibble(instruction)] = registers[helpers::getThirdNibble(instruction)] - registers[helpers::getSecondNibble(instruction)];
}

void cpu::SHL()
{
	if ((instruction & 0x8000) > 0) { registers[0xF] = 1; } // If most significant bit is 1
	else { registers[0xF] = 0; }

	registers[helpers::getSecondNibble(instruction)] *= 2;
}

void cpu::SNE_2()
{
	if (registers[helpers::getSecondNibble(instruction)] != registers[helpers::getThirdNibble(instruction)])
	{
		PC += 2;
	}
}

void cpu::LD_3()
{
	I = helpers::getLastThreeNibbles(instruction);
}

void cpu::JP_2()
{
	PC = helpers::getLastThreeNibbles(instruction) + registers[0];
}

void cpu::RND()
{
	registers[helpers::getSecondNibble(instruction)] = (rand() % 255) & helpers::getSecondByte(instruction);
}

void cpu::DRW()
{
	uint8_t sprite_size = helpers::getFourthNibble(instruction);

	for (int i = 0; i < sprite_size; i++)
	{
		uint8_t new_byte = bus_link->memory.read(I + i);

		uint8_t y_pos = registers[helpers::getThirdNibble(instruction) % bus_link->display_res_y];
		uint8_t x_pos = registers[helpers::getSecondNibble(instruction) % bus_link->display_res_x];

		for (int j = 0; j < 8; j++)
		{
			if (bus_link->display[y_pos + i][x_pos + j] && (new_byte >> 7 - j) & 0x0001)
				registers[0xF] = 1;
			else
				registers[0xF] = 0;

			bus_link->display[y_pos + i][x_pos + j] ^= (new_byte >> 7 - j) & 0x0001;
		}
	}
}

void cpu::SKP()
{
	if (bus_link->currentHeldKey == registers[helpers::getSecondNibble(instruction)])
	{
		PC += 2;
	}
}

void cpu::SKNP()
{
	if (bus_link->currentHeldKey != registers[helpers::getSecondNibble(instruction)])
	{
		PC += 2;
	}
}

void cpu::LD_4()
{
	registers[helpers::getSecondNibble(instruction)] = delayTimer;
}

void cpu::LD_5()
{
	while (bus_link->currentHeldKey == -1) {}
	registers[helpers::getSecondNibble(instruction)] = bus_link->currentHeldKey;
}

void cpu::LD_6()
{
	delayTimer = registers[helpers::getSecondNibble(instruction)];
}

void cpu::LD_7()
{
	soundTimer = registers[helpers::getSecondNibble(instruction)];
}

void cpu::ADD_3()
{
	I += registers[helpers::getSecondNibble(instruction)];
}

void cpu::LD_8()
{
	I = registers[helpers::getSecondNibble(instruction)] * 5;
}

void cpu::LD_9()
{
	bus_link->memory.write(I, (registers[helpers::getSecondNibble(instruction)] / 100) % 10); // Hundreds
	bus_link->memory.write(I + 1, (registers[helpers::getSecondNibble(instruction)] / 10) % 10); // tens
	bus_link->memory.write(I + 2, registers[helpers::getSecondNibble(instruction)] % 10); // ones
}

void cpu::LD_10()
{
	for (int i = 0; i <= helpers::getSecondNibble(instruction); i++)
	{
		bus_link->memory.write(I + i, registers[i]);
	}
}

void cpu::LD_11()
{
	for (int i = 0; i <= helpers::getSecondNibble(instruction); i++)
	{
		registers[i] = bus_link->memory.read(I + i);
	}
}
