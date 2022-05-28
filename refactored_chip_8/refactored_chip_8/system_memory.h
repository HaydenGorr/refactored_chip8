#pragma once
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

class SystemMemory {
public:
	// Memory
	uint8_t memory[0xFFF];
	const static uint16_t min_range = 0x200;
	const static uint16_t max_range = 0xFFF;

public:
	SystemMemory();
	~SystemMemory();

	void write(uint16_t, uint16_t);
	uint16_t read(uint16_t);
	void resetMemory(bool, bool);

	bool streamLoadFromFile(const std::string&);

private:
	void force_write(uint16_t, uint16_t);

};