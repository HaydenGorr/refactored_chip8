#pragma once
#include <cstdint>
#include <string>

#include "cpu.h"
#include "system_memory.h"

#include "olcPixelGameEngine.h"

struct theme {
	std::string name;
	olc::Pixel primary;
	olc::Pixel seconday;
	bool operator==(const theme& rhs)
	{
		return (name == rhs.name);
	}
};

class bus {
public:
	bus();
	~bus();

public:
	cpu chip8Sys;
	SystemMemory memory;

	const uint8_t display_res_x = 64;
	const uint8_t display_res_y = 32;

	bool display[32][64];

	theme themes[4] = {
		theme { "Macintosh", olc::Pixel(51, 51, 25, 255), olc::Pixel(229, 255, 255, 255)},
		theme { "Zenith", olc::Pixel(63, 41, 30, 255), olc::Pixel(253, 202, 85, 255)},
		theme { "IBM 8053", olc::Pixel(46,48,55,255), olc::Pixel(235,229,206,255)},
		theme { "Commodore 1084", olc::Pixel(64,49,142,255), olc::Pixel(136,215,222,255)}
	};

	theme& currentTheme;

	uint8_t lastKeyPress;
	uint8_t currentHeldKey;
};
