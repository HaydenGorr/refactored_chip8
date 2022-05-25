#include <string>
#include <iostream>
#include <bitset>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "system_memory.h"
#include "bus.h"

#include <fstream>

void loadROM(bus& busRef, const std::string inputFile)
{
	std::ifstream file(inputFile, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);

	if (size + busRef.memory.min_range <= +busRef.memory.max_range)
		file.read((char*)busRef.memory.memory + busRef.memory.min_range, size);

	//if (file.read(buffer.data(), size))
	//{
	//	uint16_t tempPTR = 0x200;
	//	for (int i = 0; i < buffer.size(); i+=2)
	//	{ 
	//		busRef.memory.write(tempPTR++, instruction);
	//	}
	//}
}

class chip8System : public olc::PixelGameEngine
{
public:
	chip8System()
	{
		sAppName = "Chip 8 Emulator";
	}

	bus whole_system;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		for (uint8_t y = 0; y < whole_system.display_res_y; y++)
		{
			for (uint8_t x = 0; x < whole_system.display_res_x; x++)
			{
				if (whole_system.display[y][x] == true)
				{
					Draw(x, y, whole_system.currentTheme.primary);
				}
				else
				{
					Draw(x, y, whole_system.currentTheme.seconday);
				}
			}
		}

		if (GetKey(olc::Key::LEFT).bPressed)
		{
			whole_system.chip8Sys.run();
		}

		return true;
	}
};

int main()
{
	chip8System newSystem;

	loadROM(newSystem.whole_system, "C:\\Users\\hayde\\Downloads\\test_opcode.ch8");

	uint16_t newPTR = 0x200;

	//// Write the bits to be displayed
	//newSystem.whole_system.memory.write(0x020F, 0x00);
	//newSystem.whole_system.memory.write(0x020F, 0xAB);

	//// Write location of bits to be displayed to I register
	//newSystem.whole_system.memory.write(newPTR++, 0xA2);
	//newSystem.whole_system.memory.write(newPTR++, 0x0F);
	//// read the location of where the bits should be displayed (0,0)
	//newSystem.whole_system.memory.write(newPTR++, 0x60);
	//newSystem.whole_system.memory.write(newPTR++, 0x02);
	//newSystem.whole_system.memory.write(newPTR++, 0x61);
	//newSystem.whole_system.memory.write(newPTR++, 0x02);
	//// Draw to the screen
	//newSystem.whole_system.memory.write(newPTR++, 0xD0);
	//newSystem.whole_system.memory.write(newPTR++, 0x18);

	if (newSystem.Construct(newSystem.whole_system.display_res_x, newSystem.whole_system.display_res_y, 16, 16))
		newSystem.Start();

	//uint8_t a = 1, b = 255;
	//std::bitset<8> x1(a), x2(b);

	//std::cout << x1 << '\n' << x2 << std::endl;

	/*uint16_t test1 = 0x9F30;
	uint16_t test2 = test1 & 0x0F00
		;
	std::cout << std::hex << test2 << std::endl;

	std::bitset<16> before(test1), after(test2);

	std::cout << before << '\n' << after << std::endl;*/


	/*uint16_t a = 0xFFFF;
	std::bitset<8> x(a);

	std::cout << x << std::endl;

	x = x << 4;

	std::cout << x << std::endl;*/

	return 0;
}



