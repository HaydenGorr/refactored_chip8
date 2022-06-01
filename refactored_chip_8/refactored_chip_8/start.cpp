#define PGE_GFX_OPENGL33
#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

#include <string>
#include <iostream>
#include <bitset>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "system_memory.h"
#include "bus.h"
#include "imgui_impl_pge.h"
#include "memory_viewer.h"
#include "ui.h"
#include "access_filesystem.h"

class Example : public olc::PixelGameEngine
{
private:
	olc::imgui::PGE_ImGUI pge_imgui;
	int m_GameLayer;

	bool toggleDebug = true;
	ui entire_ui;

	float fTargetFrameTime = 1.0f / 30.0f; // Virtual FPS of 60fps
	float fAccumulatedTime = 0.0f;

	bus chip8;

public:
	//PGE_ImGui can automatically call the SetLayerCustomRenderFunction by passing
	//true into the constructor.  false is the default value.
	Example() : pge_imgui(false), entire_ui{ chip8 }
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

		if(toggleDebug)
			drawImGuiWindows();

		// called once per frame
		for (uint8_t y = 0; y < chip8.display_res_y; y++)
		{
			for (uint8_t x = 0; x < chip8.display_res_x; x++)
			{
				if (chip8.display[y][x] == true)
				{
					Draw(x, y, chip8.currentTheme->p32);
				}
				else
				{
					Draw(x, y, chip8.currentTheme->s32);
				}
			}
		}

		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime >= fTargetFrameTime)
		{
			//if (GetKey(olc::Key::SPACE).bHeld)
			chip8.chip8Sys.run();

			fAccumulatedTime -= fTargetFrameTime;
			fElapsedTime = fTargetFrameTime;
		}

		if(GetKey(olc::Key::ENTER).bPressed)
			chip8.chip8Sys.run();

		if (GetKey(olc::Key::T).bPressed)
			toggleDebug = !toggleDebug;

		return true;
	}

	void DrawUI(void) {
		//This finishes the Dear ImGui and renders it to the screen
		pge_imgui.ImGui_ImplPGE_Render();
	}

private:
	void drawImGuiWindows() {
		//debug_memoryViewer.DrawWindow("Memory viewer ", &chip8.memory.memory, 0xFFF, 0x0);
		//fs_viewer.createWindow();
		entire_ui.draw();

		return;
		
		//ImGui::Begin("Registers");

		//for (uint8_t i = 0; i < 16; i++)
		//{
		//	std::stringstream ss;
		//	ss << "[" << std::hex << static_cast<int>(i) << "]" << " = " << std::hex << static_cast<int>(chip8.chip8Sys.registers[i]);
		//	const std::string tmp = std::string{ ss.str() };
		//	const char* str = tmp.c_str();

		//	if (chip8.chip8Sys.registers[i] == 0)
		//		ImGui::TextDisabled(str);
		//	else
		//		ImGui::Text(str, i);

		//}


		//std::stringstream ss1;
		//ss1 << "  [I]" << " = " << std::hex << static_cast<int>(chip8.chip8Sys.I);
		//const std::string tmp1 = std::string{ ss1.str() };
		//const char* str1 = tmp1.c_str();

		//ImGui::Text(str1, 17);


		//ImGui::End();

		//ImGui::Begin("Stack");

		//std::stringstream ss;
		//ss << "Stack Pointer: " << static_cast<int>(chip8.chip8Sys.getSP());
		//const std::string tmp = std::string{ ss.str() };
		//const char* str = tmp.c_str();
		//ImGui::TextColored(ImVec4(1, 1, 0, 1), str);

		//for (uint8_t i = 0; i < 16; i++)
		//{
		//	std::stringstream ss;
		//	if (chip8.chip8Sys.getSP() == i)
		//		ss << "- [" << std::hex << static_cast<int>(i) << "]" << " = " << std::hex << static_cast<int>(chip8.chip8Sys.stack[i]);
		//	else
		//		ss << "  [" << std::hex << static_cast<int>(i) << "]" << " = " << std::hex << static_cast<int>(chip8.chip8Sys.stack[i]);
		//	const std::string tmp = std::string{ ss.str() };
		//	const char* str = tmp.c_str();

		//	if (chip8.chip8Sys.stack[i] == 0)
		//		ImGui::TextDisabled(str);
		//	else
		//		ImGui::Text(str, i);
		//}
		//ImGui::End();


		//ImGui::Begin("Themes");

		//for (theme& c_theme : chip8.themes) {
		//for (int i=0; i<5; i++){
		//	const char* str = chip8.themes[i].name.c_str();

		//	ImGui::Selectable(str, chip8.currentTheme == chip8.themes[i]);

		//	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
		//	{
		//		chip8.currentTheme = chip8.themes[i];
		//	}
		//}


		//ImGui::End();
		
	}
};

int main() {
	Example demo;

	readConfigIni();

	// We add 12 to the resolution to give the chip8 screen a large border
	// which we need to give room to the debug window. and because it looks good
	if (demo.Construct(64, 32, 16, 16))
		demo.Start();

	return 0;
}


