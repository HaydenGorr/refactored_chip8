#include "bus.h"

bus::bus() : currentTheme{ themes[0] }, chip8Sys(this), currentHeldKey{ -1 }, lastKeyPress{ -1 }
{
}

bus::~bus()
{
}
