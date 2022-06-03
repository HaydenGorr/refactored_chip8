#include "bus.h"

bus::bus() : 
	chip8Sys(this), 
	currentHeldKey{ 0xFF }, 
	lastKeyPress{ 0xFF }, 
	themes{ theme{"Macintosh", t_vect4{51, 51, 25, 255}, t_vect4{229, 255, 255, 255 }} },
	currentTheme{ themes[0] }
{
	themes.clear();

	loadThemesFromJson(*this);

	currentTheme = themes[0];

}

bus::~bus()
{
}

void bus::reset()
{
	chip8Sys.reset();
	memset(display, 0, 32 * 64); // reset display
	memory.resetMemory(true, true);
}
