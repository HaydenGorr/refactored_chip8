#include "bus.h"

bus::bus() : currentTheme{ themes[0] }, chip8Sys(this), currentHeldKey{ 0xFF }, lastKeyPress{ 0xFF }
{
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
