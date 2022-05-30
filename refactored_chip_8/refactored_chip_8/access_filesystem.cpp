#include "access_filesystem.h"
#include <iostream>

std::vector<std::vector<std::string>> getAllFilesFromDirectory() {

	std::vector<std::vector<std::string>> chip8_files_in_dir;


	// Get local directory
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, sizeof(buffer));
	std::string path = fs::path(buffer).parent_path().string();

	for (const auto& entry : fs::directory_iterator(path))
	{
		std::string extension = entry.path().extension().string();
		if (extension == ".c8" || extension == ".ch8") {
			chip8_files_in_dir.push_back(std::vector<std::string>{ entry.path().string(), entry.path().filename().string() });
		}
	}
	return chip8_files_in_dir;
}