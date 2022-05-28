#include "system_memory.h"

namespace chip8_sprites {
	uint8_t char_sprites[0x10][5]{
		{ 0xF0, 0x90, 0x90, 0x90, 0xF0 },
		{ 0x20, 0x60, 0x20, 0x20, 0x70 },
		{ 0xF0, 0x10, 0xF0, 0x80, 0xF0 },
		{ 0xF0, 0x10, 0xF0, 0x10, 0xF0 },
		{ 0x90, 0x90, 0xF0, 0x10, 0x10 },
		{ 0xF0, 0x80, 0xF0, 0x10, 0xF0 },
		{ 0xF0, 0x80, 0xF0, 0x90, 0xF0 },
		{ 0xF0, 0x10, 0x20, 0x40, 0x40 },
		{ 0xF0, 0x90, 0xF0, 0x90, 0xF0 },
		{ 0xF0, 0x90, 0xF0, 0x10, 0xF0 },
		{ 0xF0, 0x90, 0xF0, 0x90, 0x90 },
		{ 0xE0, 0x90, 0xE0, 0x90, 0xE0 },
		{ 0xF0, 0x80, 0x80, 0x80, 0xF0 },
		{ 0xE0, 0x90, 0x90, 0x90, 0xE0 },
		{ 0xF0, 0x80, 0xF0, 0x80, 0xF0 },
		{ 0xF0, 0x80, 0xF0, 0x80, 0x80 }
	};
}

namespace default_loop_program {

}

SystemMemory::SystemMemory()
{
	// Load standard sprites
	uint16_t loadPtr = 0x0;
	for (auto& sprite : chip8_sprites::char_sprites)
		for (auto& row : sprite)
			force_write(loadPtr++, row);

	// Load the program loop
	// This will cause the system to remain "on" without a rom loaded
	write(0x200, 0x00);
	write(0x200, 0x12);
}

SystemMemory::~SystemMemory()
{
}

void SystemMemory::write(uint16_t in_addr, uint16_t data)
{
	if (in_addr <= max_range && in_addr >= min_range)
		memory[in_addr] = data;
}

uint16_t SystemMemory::read(uint16_t in_addr)
{
	if (in_addr <= max_range)
		return memory[in_addr];
}

void SystemMemory::resetMemory()
{
	std::memset(memory, 0, sizeof(memory));
}

bool SystemMemory::streamLoadFromFile(const std::string& fileURL)
{
	std::ifstream file(fileURL, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);

	if (size + min_range <= +max_range)
		file.read((char*)memory + min_range, size);

	return false;
}

void SystemMemory::force_write(uint16_t in_addr, uint16_t data)
{
	memory[in_addr] = data;
}
