#include "File_Select_UI.h"
#include "imgui.h"

#include <iostream>

File_Select_UI::File_Select_UI() {
	updateFileList();
}

void File_Select_UI::updateFileList() {

	current_files = getAllFilesFromDirectory();

	// Load the file names from current_files
	for (auto& file : current_files) {
		fileNames.push_back(file[1].c_str());
	}


}

//void File_Select_UI::drawUI(static int currentFile) {
//
//	ImGui::Text("Currently playing ");
//	ImGui::SameLine();
//
//	ImGui::TextColored(ImVec4(255, 50, 0, 255), (currentFile > -1) ? fileNames[currentFile] : "Home_loop.ch8");
//
//	if (fileNames.size() > 0) {
//		//ImGui::ListBox("ROMS", &currentFile, fileNames.data(), current_files.size(), 4);
//
//		if (ImGui::BeginListBox("ROMS"))
//		{
//			for (int n = 0; n < current_files.size(); n++)
//			{
//				const bool is_selected = (currentFile == n);
//				if (ImGui::Selectable(fileNames[n], is_selected)) {
//					item_current_idx = n;
//				}
//				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
//				if (is_selected)
//					ImGui::SetItemDefaultFocus();
//			}
//			ImGui::EndListBox();
//		}
//
//
//		if (ImGui::Button("Load")) {
//			ImGui::CloseCurrentPopup();
//			ImGui::SameLine();
//		}
//		
//
//	}
//	else {
//		ImGui::Text("No ROMS available. Put .ch8 files in the same directory of this exe");
//	}
//
//	if (ImGui::Button("Unload")) {
//		ImGui::CloseCurrentPopup();
//	}
//
//	ImGui::SameLine();
//
//	if (ImGui::Button("refresh list")) {
//		updateFileList();
//	}
//	
//}