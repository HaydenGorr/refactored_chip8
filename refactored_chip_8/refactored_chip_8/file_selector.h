

#include "bus.h"

namespace fs = std::filesystem;

class file_selector {

private:
	bus& bus_Ref;
	std::vector<std::vector<std::string>> chip8_files_in_dir;

private:
	void getAllFilesFromDirectory() {

		chip8_files_in_dir.clear();

		std::string path = GetCurrentDirectory().string();
		for (const auto& entry : fs::directory_iterator(path))
		{
			std::string extension = entry.path().extension().string();
			if (extension == ".c8" || extension == ".ch8") {
				chip8_files_in_dir.push_back(std::vector<std::string>{ entry.path().string(), entry.path().filename().string() });
			}
		}

	}

	fs::path GetCurrentDirectory()
	{
		wchar_t buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, sizeof(buffer));
		return fs::path(buffer).parent_path();
	}

public:

	file_selector(bus& inRef) : bus_Ref{ inRef } {
		getAllFilesFromDirectory();
	}

	void createWindow() {
		ImGui::Begin("ROMS");

		// Draw files in a row and handle clicks
		for (auto& file_str : chip8_files_in_dir) {
			const char* str = file_str[1].c_str();
			ImGui::Selectable(str, false);

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
				bus_Ref.reset();
				bus_Ref.memory.streamLoadFromFile(file_str[0]);
			}
		}

		/*
		* Draw buttons and handle clicks
		*/

		ImGui::Separator();

		if (chip8_files_in_dir.size() > 0) {
			if (ImGui::Button("Unload")) {
				// This resets the memory but retains the chars and loads the loop program
				// Resets the CPU and all registers
				// Clears the display
				bus_Ref.reset();
			}
			ImGui::SameLine();
		}

		if (ImGui::Button("Reload")) {
			getAllFilesFromDirectory();
		}

		ImGui::End();
	}

};