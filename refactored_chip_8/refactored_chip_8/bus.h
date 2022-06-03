#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "cpu.h"
#include "system_memory.h"
#include "themes.h"

class bus {
public:
	bus();
	~bus();

	void reset();

public:
	cpu chip8Sys;
	SystemMemory memory;

	const uint8_t display_res_x = 64;
	const uint8_t display_res_y = 32;

	bool display[32][64];

	std::vector<theme> themes;

	// we copy the theme to this var for performance. 
	// It's accessed many times a second, this keeps the current theme nearby in memory
	theme currentTheme;

	uint8_t lastKeyPress;
	uint8_t currentHeldKey;

	int currentlyLoadedRom = -1;

};
