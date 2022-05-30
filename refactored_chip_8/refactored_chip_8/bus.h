#pragma once
#include <cstdint>
#include <string>

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

	theme themes[5] = {
		theme{"Macintosh", t_vect4{51, 51, 25, 255}, t_vect4{229, 255, 255, 255 }},
		theme{"Zenith", t_vect4{63, 41, 30, 255}, t_vect4{253, 202, 85, 255 }},
		theme{"IBM 8053", t_vect4{ 46, 48, 55, 255 }, t_vect4{ 235, 229, 206, 255 }},
		theme{"Commodore 1084", t_vect4{64, 49, 142, 255}, t_vect4{136, 215, 222, 255}},
		theme{"IBM 5151", t_vect4{37, 51, 47, 255}, t_vect4{1, 235, 97, 255}}
	};

	theme& currentTheme;

	uint8_t lastKeyPress;
	uint8_t currentHeldKey;

	int currentlyLoadedRom = -1;

};
