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

cpu::cpu(bus* in_bus_link) : PC{ 0x200 }, SP{ 0xFF }, delayTimer{ 0 }, soundTimer{ 0 }, bus_link{ in_bus_link }
{
	srand(time(NULL));
}

cpu::~cpu()
{
}


#include <bitset>

void cpu::run()
{
	instruction = ((uint16_t)bus_link->memory.read(PC++) << 8) | ((uint16_t)bus_link->memory.read(PC++));

	uint16_t id_1 = helpers::getFirstNibble(instruction), id_2 = helpers::getFourthNibble(instruction), id_3 = helpers::getThirdNibble(instruction);

#ifdef chip_debug
	std::cout << "PC: " << PC << std::endl;
	std::cout << "instruction: " << std::hex << instruction << std::endl;
#endif

	uint16_t firstN = helpers::getFirstNibble(instruction), thirdN = helpers::getThirdNibble(instruction), fourthN = helpers::getFourthNibble(instruction), lastByte = helpers::getSecondByte(instruction);

	switch (firstN)
	{
	case 0:
		switch (instruction)
		{
		case 0x00E0:
			std::cout << "CLS()" << std::endl;
			CLS();
			break;
		case 0x00EE:
			std::cout << "RET()" << std::endl;
			RET();
			break;
		default:
			std::cout << "SYS()" << std::endl;
			SYS();
			break;
		}
		break;
	case 1:
		std::cout << "JP_1()" << std::endl;
		JP_1();
		break;
	case 2:
		std::cout << "CALL()" << std::endl;
		CALL();
		break;
	case 3:
		std::cout << "SE_1()" << std::endl;
		SE_1();
		break;
	case 4:
		std::cout << "SNE_1()" << std::endl;
		SNE_1();
		break;
	case 5:
		std::cout << "SE_2()" << std::endl;
		SE_2();
		break;
	case 6:
		std::cout << "LD_1()" << std::endl;
		LD_1();
		break;
	case 7:
		std::cout << "ADD_1()" << std::endl;
		ADD_1();
		break;
	case 8:
		switch (fourthN)
		{
		case 0:
			std::cout << "LD_2()" << std::endl;
			LD_2();
			break;
		case 1:
			std::cout << "OR()" << std::endl;
			OR();
			break;
		case 2:
			std::cout << "AND()" << std::endl;
			AND();
			break;
		case 3:
			std::cout << "XOR()" << std::endl;
			XOR();
			break;
		case 4:
			std::cout << "ADD_2()" << std::endl;
			ADD_2();
			break;
		case 5:
			std::cout << "SUB()" << std::endl;
			SUB();
			break;
		case 6:
			std::cout << "SHR()" << std::endl;
			SHR();
			break;
		case 7:
			std::cout << "SUBN()" << std::endl;
			SUBN();
			break;
		case 0xE:
			std::cout << "SHL()" << std::endl;
			SHL();
			break;
		}
		break;
	case 9:
		std::cout << "SNE_2()" << std::endl;
		SNE_2();
		break;
	case 0xA:
		std::cout << "LD_3()" << std::endl;
		LD_3();
		break;
	case 0xB:
		std::cout << "JP_2()" << std::endl;
		JP_2();
		break;
	case 0xC:
		std::cout << "RND()" << std::endl;
		RND();
		break;
	case 0xD:
		std::cout << "DRW()" << std::endl;
		DRW();
		break;
	case 0xE:
		switch (fourthN)
		{
		case 0xE:
			std::cout << "SKP()" << std::endl;
			SKP();
			break;
		case 0x1:
			std::cout << "SKNP()" << std::endl;
			SKNP();
			break;
		}
		break;
	case 0xF:
		switch (lastByte)
		{
		case 0x07:
			std::cout << "LD_4()" << std::endl;
			LD_4();
			break;
		case 0x0A:
			std::cout << "LD_5()" << std::endl;
			LD_5();
			break;
		case 0x15:
			std::cout << "LD_6()" << std::endl;
			LD_6();
			break;
		case 0x18:
			std::cout << "LD_7()" << std::endl;
			LD_7();
			break;
		case 0x1E:
			std::cout << "ADD_3()" << std::endl;
			ADD_3();
			break;
		case 0x29:
			std::cout << "LD_8()" << std::endl;
			LD_8();
			break;
		case 0x33:
			std::cout << "LD_9()" << std::endl;
			LD_9();
			break;
		case 0x55:
			std::cout << "LD_10()" << std::endl;
			LD_10();
			break;
		case 0x65:
			std::cout << "LD_11()" << std::endl;
			LD_11();
			break;
		}
		break;
	}

	std::cout << "\n---\n" << std::endl;
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
	PC = SP--;
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
	registers[helpers::getSecondNibble(instruction)] = (helpers::getSecondByte(instruction));
}

void cpu::ADD_1()
{
	registers[helpers::getSecondNibble(instruction)] += registers[helpers::getSecondByte(instruction)];
}

void cpu::LD_2()
{
	registers[helpers::getSecondNibble(instruction)] = registers[helpers::getThirdNibble(instruction)];
}

void cpu::OR()
{
	registers[helpers::getSecondNibble(instruction)] = registers[helpers::getSecondNibble(instruction)] | registers[(helpers::getThirdNibble(instruction))];
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

	if (result > 0xFF) { VF = true; }
	else { VF = false; }

	registers[helpers::getSecondNibble(instruction)] = (uint8_t)result & 0x00FF;
}

void cpu::SUB()
{
	if (registers[helpers::getSecondNibble(instruction)] > registers[helpers::getThirdNibble(instruction)]) { VF = true; }
	else { VF = false; }

	registers[helpers::getSecondNibble(instruction)] -= helpers::getThirdNibble(instruction);
}

void cpu::SHR()
{
	if ((instruction & 0x0001) > 0) { VF = true; } // If least significant bit is 1
	else { VF = false; }

	registers[helpers::getSecondNibble(instruction)] /= 2;
}

void cpu::SUBN()
{
	if (registers[helpers::getThirdNibble(instruction)] > registers[helpers::getSecondNibble(instruction)]) { VF = true; }
	else { VF = false; }

	registers[helpers::getSecondNibble(instruction)] = helpers::getThirdNibble(instruction) - helpers::getSecondNibble(instruction);
}

void cpu::SHL()
{
	if ((instruction & 0x8000) > 0) { VF = true; } // If most significant bit is 1
	else { VF = false; }

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
			bus_link->display[y_pos + i][x_pos + j] = (new_byte >> 7 - j) & 0x0001;
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
	for (int i = 0; i < helpers::getSecondNibble(instruction); i++)
	{
		bus_link->memory.write(I + i, registers[i]);
	}
}

void cpu::LD_11()
{
	for (int i = 0; i < helpers::getSecondNibble(instruction); i++)
	{
		registers[i] = bus_link->memory.read(I + i);
	}
}
