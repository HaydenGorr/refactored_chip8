#define PGE_GFX_OPENGL33
#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#define OLC_PGE_APPLICATION

#include <string>
#include <iostream>
#include <bitset>
#include <fstream>

#include "olcPixelGameEngine.h"
#include "system_memory.h"
#include "bus.h"
#include "imgui_impl_pge.h"

#include "memory_viewer.h"

void loadROM(bus& busRef, const std::string inputFile)
{
	std::ifstream file(inputFile, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);

	if (size + busRef.memory.min_range <= +busRef.memory.max_range)
		file.read((char*)busRef.memory.memory + busRef.memory.min_range, size);
}


void create_imGui_ui() {

	MemoryEditor asd;

	///asd.DrawWindow("memviewer", ,0xFFF ,0x200);

}

class Example : public olc::PixelGameEngine
{
	olc::imgui::PGE_ImGUI pge_imgui;
	int m_GameLayer;
	MemoryEditor asd;

public:
	bus chip8;

public:
	//PGE_ImGui can automatically call the SetLayerCustomRenderFunction by passing
	//true into the constructor.  false is the default value.
	Example() : pge_imgui(false)
	{
		sAppName = "Test Application";
	}

public:

	bool OnUserCreate() override
	{
		//Create a new Layer which will be used for the game
		m_GameLayer = CreateLayer();
		//The layer is not enabled by default,  so we need to enable it
		EnableLayer(m_GameLayer, true);

		//Set a custom render function on layer 0.  Since DrawUI is a member of
		//our class, we need to use std::bind
		//If the pge_imgui was constructed with _register_handler = true, this line is not needed
		SetLayerCustomRenderFunction(0, std::bind(&Example::DrawUI, this));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//Change the Draw Target to not be Layer 0
		SetDrawTarget((uint8_t)m_GameLayer);
		//Game Drawing code here

		//Create and react to your UI here, it will be drawn during the layer draw function
		asd.DrawWindow("Memory viewer ", &chip8.memory.memory, 0xFFF, 0x0);

		// called once per frame
		for (uint8_t y = 0; y < chip8.display_res_y; y++)
		{
			for (uint8_t x = 0; x < chip8.display_res_x; x++)
			{
				if (chip8.display[y][x] == true)
				{
					Draw(x, y, chip8.currentTheme.primary);
				}
				else
				{
					Draw(x, y, chip8.currentTheme.seconday);
				}
			}
		}

		if (GetKey(olc::Key::LEFT).bPressed)
		{
			chip8.chip8Sys.run();
		}

		return true;
	}

	void DrawUI(void) {
		//This finishes the Dear ImGui and renders it to the screen
		pge_imgui.ImGui_ImplPGE_Render();
	}
};

int main() {
	Example demo;

	loadROM(demo.chip8, "C:\\Users\\hayde\\Downloads\\test_opcode.ch8");

	// We add 12 to the resolution to give the chip8 screen a large border
	// which we need to give room to the	debug window. and because it looks good
	if (demo.Construct(64, 32, 16, 16))
		demo.Start();


	return 0;
}


