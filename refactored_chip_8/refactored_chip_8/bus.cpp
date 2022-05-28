#include "bus.h"

bus::bus() : currentTheme{ themes[0] }, chip8Sys(this), currentHeldKey{ 0xFF }, lastKeyPress{ 0xFF }
{
}

bus::~bus()
{
}
