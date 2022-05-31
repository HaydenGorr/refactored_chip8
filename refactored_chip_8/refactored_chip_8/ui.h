#pragma once
#include "access_filesystem.h"
#include "File_Select_UI.h"
#include "bus.h"
#include "themes.h"


#define Y_HIGHLIGHTED_TEXT(a) ImGui::TextColored(ImVec4(255, 50, 0, 255), a);


class ui {
public:
	std::vector<std::vector<std::string>> current_files;
	std::vector<const char*> fileNames;

    bus& mainsystem;


    ui(bus& inBus) : mainsystem { inBus } {
		updateFileList();
	}

public:
    void draw() {



        //drawMenuBar();
        ImGui::ShowDemoWindow();

        if (!ImGui::Begin("Chip8 control panel"))
        {
            ImGui::End();
            return;
        }

		if (ImGui::CollapsingHeader("Load ROM"))
		{
			drawLoadROM();
			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Theme picker"))
		{
			drawThemeselector();
			ImGui::Separator();
		}

        ImGui::End();
    }





    void drawLoadROM() {

		Y_HIGHLIGHTED_TEXT("Currently playing: ");
		ImGui::SameLine();
		Y_HIGHLIGHTED_TEXT((mainsystem.currentlyLoadedRom > -1) ? fileNames[mainsystem.currentlyLoadedRom] : "Home loop");

		if (fileNames.size() > 0) {
			if (ImGui::BeginListBox("ROMS", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()) )) // max list size = 5 elements
			{
				for (int n = 0; n < current_files.size(); n++)
				{
					const bool is_selected = (mainsystem.currentlyLoadedRom == n);
					if (ImGui::Selectable(fileNames[n], is_selected)) {
						mainsystem.currentlyLoadedRom = n;
						mainsystem.reset();
						mainsystem.memory.streamLoadFromFile(current_files[n][0]);
					}
					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

		}
		else {
			ImGui::Text("No ROMS available. Put .ch8 files in the same directory of this exe");
		}

		if (ImGui::Button("Unload")) {
			mainsystem.reset();
		}

		ImGui::SameLine();

		if (ImGui::Button("refresh list")) {
			updateFileList();
		}
    }


	void drawThemeselector() {
		{
			Y_HIGHLIGHTED_TEXT("Current theme: ");
			ImGui::SameLine();
			Y_HIGHLIGHTED_TEXT((*mainsystem.currentTheme).name.c_str());

			for (int i = 0; i<mainsystem.themes.size(); i++)
			{
				if (ImGui::Selectable(mainsystem.themes[i].name.c_str())){
					mainsystem.currentTheme = &mainsystem.themes[i];
				};

				if (ImGui::BeginPopupContextItem())
				{
					ImGui::Text("Edit colours", mainsystem.themes[i].name.c_str());
					ImGui::Separator();
					ImGui::Text("Primary Colour"); 
					ImGui::SameLine();
					ImGui::ColorEdit4("MyColor##1", mainsystem.themes[i].p_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::Text("Secondary Colour");
					ImGui::SameLine();
					ImGui::ColorEdit4("MyColor##2", mainsystem.themes[i].s_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					if (ImGui::Button("Close"))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
					mainsystem.themes[i].convertPcol_top32();
					mainsystem.themes[i].convertscol_tos32();
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Right-click to open popup");
			}
		}





	}




private:
	void updateFileList() {

		current_files = getAllFilesFromDirectory();

		fileNames.clear();

		// Load the file names from current_files
		for (auto& file : current_files) {
			fileNames.push_back(file[1].c_str());
		}

	}
};
