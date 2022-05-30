#pragma once
#include "access_filesystem.h"
#include "File_Select_UI.h"
#include "bus.h"
#include "themes.h"


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
		ImGui::Text("Currently playing ");
		ImGui::SameLine();

		ImGui::TextColored(ImVec4(255, 50, 0, 255), (mainsystem.currentlyLoadedRom > -1) ? fileNames[mainsystem.currentlyLoadedRom] : "Home loop");

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
		static ImVec4 Pcolor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
		static ImVec4 Scolor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
		//ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);


		{
			const char* names[5] = { "Label1", "Label2", "Label3", "Label4", "Label5" };
			for (auto& theme : mainsystem.themes)
			{
				ImGui::Selectable(theme.name.c_str());

				if (ImGui::BeginPopupContextItem())
				{
					ImGui::Text("Edit colours", theme.name.c_str());
					ImGui::Separator();
					ImGui::Text("Primary Colour");
					ImGui::SameLine();
					ImGui::ColorEdit4("MyColor##1", (float*)&theme.p_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::Text("Secondary Colour");
					ImGui::SameLine();
					ImGui::ColorEdit4("MyColor##2", (float*)&theme.s_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					if (ImGui::Button("Close"))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
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
