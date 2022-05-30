#pragma once
#include <filesystem>
#include <Windows.h>

namespace fs = std::filesystem;

std::vector<std::vector<std::string>> getAllFilesFromDirectory();