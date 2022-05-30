#pragma once
#include <cstdint>
#include <string>

#include "cpu.h"
#include "system_memory.h"
#include "directxmath.h"

#include "olcPixelGameEngine.h"

struct colour_vec
{
	float r, g, b, a;
};

struct theme {
	std::string name;
	olc::Pixel primary;
	olc::Pixel seconday;
	float IM_primaryColour[4];
	float IM_secondColour[4];

	theme(std::string inName, colour_vec P, colour_vec S) : name{inName}, primary { P.r, P.g, P.b, P.a }, seconday{ olc::Pixel(S.r, S.g, S.b, S.a) }, IM_primaryColour{ P.r, P.g, P.b, P.a }, IM_secondColour{ S.r, S.g, S.b, S.a }{}

	bool operator==(const theme& rhs)
	{
		return (name == rhs.name);
	}

	void setPColour() {

	}
};

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

	theme themes[4] = {
		theme ( "Macintosh", colour_vec{51, 51, 25, 255}, colour_vec{229, 255, 255, 255} ),
		theme ( "Zenith", colour_vec{63, 41, 30, 255}, colour_vec{253, 202, 85, 255 }),
		theme ( "IBM 8053", colour_vec{46,48,55,255 }, colour_vec{235, 229, 206, 255}),
		theme ( "Commodore 1084", colour_vec{64,49,142,255 }, colour_vec{136, 215, 222, 255})
	};

	theme& currentTheme;

	uint8_t lastKeyPress;
	uint8_t currentHeldKey;

	int currentlyLoadedRom = -1;

};
