#ifndef VN_ENGINE_CHARACTERS_CONFIG_H
#define VN_ENGINE_CHARACTERS_CONFIG_H

#include "state_subject.h"
#include "engine_helpers.h"
#include "character.h"
#include "character_builder.h"
#include "model_engine_interface.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <glm/glm.hpp>

#include <codecvt>
#include <iostream>

class VnEngineCharacterEditor {
private:
	static StateSubject* m_stateSubject;

public:
	VnEngineCharacterEditor(StateSubject* stateSubject)
	{
		m_stateSubject = stateSubject;
	}

	~VnEngineCharacterEditor() {}

private:
	static Character m_addableCharacter;

	// ptrSeed for making each text and color field unique
	bool drawCharacter(Character* character) {
		if (character == nullptr) { return false; }

		ImGui::Text("Character Name: ");
		ImGui::SameLine();
		std::string convertedName{ myconv.to_bytes(character->getName()) };
		bool nameModified = ImGui::InputText(addIdFromPtr("##nodeName", character).c_str(), &convertedName);
		if (nameModified) { CharacterBuilder{ character }.setName(myconv.from_bytes(convertedName)); }

		ImGui::Text("Speech Color: ");
		ImGui::SameLine();
		glm::vec3 tempColor{ character->getTextColor() };
		bool colorModified = ImGui::ColorEdit3(addIdFromPtr("##colorPicker", character).c_str(), (float*)&tempColor, 0);
		if (colorModified) { CharacterBuilder{ character }.setTextColor(tempColor); }
	
		return colorModified || nameModified;
	}

public:
	void draw() {
		auto& characters = ModelEngineInterface::getCharacterMap();
		
		ImGui::SeparatorText("Add Character");
		drawCharacter(&m_addableCharacter);
		bool addClicked = ImGui::Button("Add##AddCharacterToCharactersFromEditor", ImVec2(260.0f, 0.0f));
		if (addClicked) {
			std::unique_ptr<Character> allocatedCharacter{ std::make_unique<Character>(m_addableCharacter) };

			characters.insert({ allocatedCharacter.get()->getId(), std::move(allocatedCharacter) });
		}

		ImGui::Spacing();
		ImGui::SeparatorText("Edit Existing Characters");
	
		bool existingModified{ false };
		for (auto iter = characters.begin(); iter != characters.end(); iter++) {
			existingModified |= drawCharacter(iter->second.get());

			bool deleteClicked = ImGui::Button(addIdFromPtr("Delete##DeleteCharacterThroughEditor", iter->second.get()).c_str(), ImVec2(260.0f, 0.0f));
			if (deleteClicked) {
				existingModified = true;
				std::cout << iter->first << std::endl;
				iter = characters.erase(iter);
				if (iter == characters.end()) { break; }
			}

			ImGui::Spacing();
		}

		// Reload state to current step to update character properties in scene
		if (existingModified && m_stateSubject != nullptr) {
			m_stateSubject->goToStepIndex(m_stateSubject->getStepIndex());
		}
	}
};


#endif // VN_ENGINE_CHARACTERS_CONFIG_H