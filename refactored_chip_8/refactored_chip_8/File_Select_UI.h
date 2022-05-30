#include "access_filesystem.h"


class File_Select_UI {
public:
	std::vector<std::vector<std::string>> current_files;
	std::vector<const char*> fileNames;

public:
	File_Select_UI();

	void updateFileList();

public:
	void drawUI(static int);
};