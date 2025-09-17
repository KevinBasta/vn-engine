#ifndef ENGINE_CHAPTER_CONFIG_EDITOR_H
#define ENGINE_CHAPTER_CONFIG_EDITOR_H

#include "state_subject.h"
#include "engine_helpers.h"
#include "model_engine_interface.h"

#include "chapter.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <iostream>

class VnEngineChapterEditor {
private:
	static StateSubject* m_stateSubject;

public:
	VnEngineChapterEditor(StateSubject* stateSubject)
	{
		m_stateSubject = stateSubject;
	}

	~VnEngineChapterEditor() {}


public:
	void draw() {
		ModelEngineInterface::ChapterMap& chapters{ ModelEngineInterface::getChaptersMap() };


		ImGui::SeparatorText("Add New Chapter");

		// Chatper title
		ImGui::Text("Chapter Title: ");

		// Chapter head node id
		ImGui::Text("Head Node Id: ");


		// Add button


		ImGui::SeparatorText("Modify Existing Chapter");
		
		ImGui::PushItemWidth(200);
		static id currentChapterId{ chapters.begin()->first };
		std::string currentChapterName{ myconv.to_bytes(chapters.at(currentChapterId).get()->getName()) };

		if (ImGui::BeginCombo(addIdFromPtr("###chapterpickerforviewmodify", &(currentChapterId)).c_str(), currentChapterName.c_str(), 0)) {
			for (auto charactersIter{ chapters.begin() }; charactersIter != chapters.end(); charactersIter++) {
				const bool isSelected = (currentChapterId == charactersIter->first);

				std::string curCharacterName{ myconv.to_bytes(charactersIter->second.get()->getName()) };
				if (ImGui::Selectable(curCharacterName.c_str(), isSelected)) {
					currentChapterId = charactersIter->first;
				}

			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		// Modify chapter title
		ImGui::Text("Chapter Title: ");


		// Modify chapter head node id
		ImGui::Text("Head Node Id: ");


		// Delete button


	}

};


#endif // ENGINE_CHAPTER_CONFIG_EDITOR_H