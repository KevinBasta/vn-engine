#ifndef VN_ENGINE_RELATIONS_CONFIG_H
#define VN_ENGINE_RELATIONS_CONFIG_H

#include "state_subject.h"
#include "engine_helpers.h"
#include "model_engine_interface.h"

#include "relations.h"
#include "engine_helpers.h"

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

class VnEngineRelationEditor {
private:
	static StateSubject* m_stateSubject;

public:
	VnEngineRelationEditor(StateSubject* stateSubject)
	{
		m_stateSubject = stateSubject;
	}

	~VnEngineRelationEditor() {}

private:
	void drawRelationTypes() {
		ModelEngineInterface::RelationTypeMap& types{ ModelEngineInterface::getRelationTypesMap() };

		ImGui::SeparatorText("Relation Types");

		ImGui::Text("Current Types");
		ImGui::Spacing();
		for (auto iter{ types.begin() }; iter != types.end(); iter++) {
			std::string relationName{ iter->second };
			bool typeNameModified = ImGui::InputText(addIdFromPtr("##relationType", &(iter->second)).c_str(), &(relationName));
			if (typeNameModified) { types[iter->first] = relationName; }
			ImGui::SameLine();
			bool deleteClicked = ImGui::Button(addIdFromPtr("Delete##deleteRelationType", &(iter->second)).c_str(), ImVec2(70.0f, 0.0f));
			if (deleteClicked) {
				iter = types.erase(iter);

				if (iter == types.end()) {
					// modifying the map may bring us to last iterator, must break
					break;
				}
			}
		}

		ImGui::Text("Add New Relation Type");
		ImGui::Spacing();
		static std::string newRelationType{};
		bool typeNameModified = ImGui::InputText(addIdFromPtr("##relationTypeLocal", &(newRelationType)).c_str(), &(newRelationType));
		ImGui::SameLine();
		bool addClicked = ImGui::Button(addIdFromPtr("Add##addRelationType", &(newRelationType)).c_str(), ImVec2(70.0f, 0.0f));
		if (addClicked) {
			ModelEngineInterface::addRelationType(newRelationType);
		}
	}

	void drawBaseRelations() {
		ModelEngineInterface::RelationsMap& relations{ ModelEngineInterface::getRelationsMap() };
		ModelEngineInterface::RelationTypeMap& relationTypes{ ModelEngineInterface::getRelationTypesMap() };
		ModelEngineInterface::CharacterMap& characterMap{ ModelEngineInterface::getCharacterMap() };

		ImGui::SeparatorText("Starting Relations Settings");

		// Relation of
		ImGui::Text("Relation Of: ");
		ImGui::SameLine();
		ImGui::PushItemWidth(200);
		static id characterOne{ characterMap.begin()->first };
		std::string characterOneName{ myconv.to_bytes(characterMap.at(characterOne).get()->getName()) };

		if (ImGui::BeginCombo(addIdFromPtr("###RelationTypePickerForCharacterOne", &(characterOne)).c_str(), characterOneName.c_str(), 0)) {
			for (auto charactersIter{ characterMap.begin() }; charactersIter != characterMap.end(); charactersIter++) {
				const bool isSelected = (characterOne == charactersIter->first);

				std::string curCharacterName{ myconv.to_bytes(charactersIter->second.get()->getName()) };
				if (ImGui::Selectable(curCharacterName.c_str(), isSelected)) {
					characterOne = charactersIter->first;
				}

			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		// Relation With
		ImGui::Text("Relation With: ");
		ImGui::SameLine();
		ImGui::PushItemWidth(200);
		static id characterTwo{ characterMap.begin()->first };
		std::string characterTwoName{ myconv.to_bytes(characterMap.at(characterTwo).get()->getName()) };

		if (ImGui::BeginCombo(addIdFromPtr("###RelationTypePickerForCharacterTwo", &(characterTwo)).c_str(), characterTwoName.c_str(), 0)) {
			for (auto charactersIter{ characterMap.begin() }; charactersIter != characterMap.end(); charactersIter++) {
				const bool isSelected = (characterTwo == charactersIter->first);

				std::string curCharacterName{ myconv.to_bytes(charactersIter->second.get()->getName()) };
				if (ImGui::Selectable(curCharacterName.c_str(), isSelected)) {
					characterTwo = charactersIter->first;
				}

			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		// Relation Type
		ImGui::Text("Type: ");
		ImGui::SameLine();
		ImGui::PushItemWidth(200);
		static id relationTypeToAdd{ relationTypes.begin()->first }; // TODO change to non static 
		if (ImGui::BeginCombo(addIdFromPtr("###RelationTypePickerForCharacter", &(relationTypeToAdd)).c_str(), relationTypes.at(relationTypeToAdd).c_str(), 0)) {
			for (auto relationTypeIter{ relationTypes.begin() }; relationTypeIter != relationTypes.end(); relationTypeIter++) {
				const bool isSelected = (relationTypeToAdd == relationTypeIter->first);

				if (ImGui::Selectable(relationTypeIter->second.c_str(), isSelected)) {
					relationTypeToAdd = relationTypeIter->first;
				}

			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		
		// Relation Value
		ImGui::Text("Value: ");
		ImGui::SameLine();
		static int relationSettingsValue{ 0 };
		bool relationSettingsValueModified{ ImGui::DragInt(addIdFromPtr("###therelationvalue", &(relationSettingsValue)).c_str(), &(relationSettingsValue), 1, INT_MIN, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp) };

		// Add with the settings above
		bool relationTypeAddClicked{ ImGui::Button(addIdFromPtr("Add###FullRelationTypeAdder", &(relationSettingsValue)).c_str(), {50, 20}) };
		if (relationTypeAddClicked) {
			if (relations.find(characterOne) == relations.end()) {
				RelationsBuilder newRelation{};
				relations[characterOne] = std::unique_ptr<Relations>{ newRelation.get() };
			}
			
			RelationsBuilder{ relations[characterOne].get() }.setRelationValue(characterTwo, relationTypeToAdd, relationSettingsValue);
		}

		ImGui::SeparatorText("Starting Relations");

		
		// Loop through each relation of character to other characters
		for (auto iter{ relations.begin() }; iter != relations.end(); ) {
			// Attempt to find the name of the character of the relation
			Character* mainCharacterOfRelation{ ModelEngineInterface::getCharacterById(iter->first) };
			if (mainCharacterOfRelation == nullptr) { continue; } // TODO: Delete relation entry if not found

			// Display which character's relations this section is
			ImGui::Separator();
			std::string title{ "Relations of " + myconv.to_bytes(mainCharacterOfRelation->getName()) + "###id" };
			bool isTreeOpen = true;
			ImGui::SeparatorText(addIdFromPtr(title, &iter->first).c_str());

			if (isTreeOpen) {
				// Get the ptr from the unique ptr, then get the map of relations with other characters
				Relations* characterRelations{ iter->second.get() };
				if (characterRelations == nullptr) { break; }
				auto& characterToRelationMap{ RelationsBuilder{characterRelations}.getCharacterToRelationMap() };

				// Iterate through the current character's relations with other characters
				for (auto iter2{ characterToRelationMap.begin() }; iter2 != characterToRelationMap.end();) {
					// Attempt to find the name of the other character
					Character* relationWithCharacter{ ModelEngineInterface::getCharacterById(iter2->first) };
					if (relationWithCharacter == nullptr) { continue; } // TODO: Delete relation entry if not found

					// Display which character this relation is with
					std::string subtitle{ "With " + myconv.to_bytes(relationWithCharacter->getName()) };
					bool withTreeNode{ ImGui::TreeNodeEx(addIdFromPtr(subtitle.c_str(), relationWithCharacter).c_str(), 0)};
					
					if (withTreeNode) {
						auto& relationshipFeelings{ iter2->second };

						// Iterate through the differnet types of feelings in the relation
						for (auto iter3{ relationshipFeelings.begin() }; iter3 != relationshipFeelings.end();) {
							// Get the name of the relation feeling to display it
							auto relationTypeIter{ relationTypes.find(iter3->first) };
							if (relationTypeIter != relationTypes.end()) {
								ImGui::PushItemWidth(300.0f);
								ImGui::Text(relationTypeIter->second.c_str());
								ImGui::PopItemWidth();
								ImGui::SameLine();

								// Allow modification of the value of the realtion type
								int curVal{ iter3->second };
								bool curValModified{ ImGui::DragInt(addIdFromPtr("###therelationvalue", &(iter3->first)).c_str(), &(curVal), 1.0f, INT_MIN, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp) };
								if (curValModified) {
									iter2->second[iter3->first] = curVal;
								}

								ImGui::SameLine();
							}

							// Allow delete of the relation feeling
							bool relationTypeDeleteClicked{ ImGui::Button(addIdFromPtr("Delete###relationItself", &(iter3->first)).c_str()) };
							if (relationTypeDeleteClicked) {
								iter3 = iter2->second.erase(iter3);
								if (iter3 == iter2->second.end()) {
									break;
								}
							}
							else {
								iter3++;
							}
						}

						// Allow delete of relation with a character
						std::string deleteHalfRelationTitle{ "Delete Directional##" + myconv.to_bytes(relationWithCharacter->getName()) + "###relationWith" };
						bool relationWithDeleteClicked{ ImGui::Button(addIdFromPtr(deleteHalfRelationTitle.c_str(), &(iter2->first)).c_str())};
						if (relationWithDeleteClicked) {
							iter2 = characterToRelationMap.erase(iter2);
							if (iter2 == characterToRelationMap.end()) {
								ImGui::TreePop();
								break;
							}
						}
						else {
							iter2++;
						}

						ImGui::TreePop();
					}
					else {
						iter2++;
					}
				}



				// Allow delete of relation of a character
				std::string deleteFullRelationTitle{ "Delete Full##" + myconv.to_bytes(mainCharacterOfRelation->getName()) + "###relationOf" };
				bool relationDeleteClicked{ ImGui::Button(addIdFromPtr(deleteFullRelationTitle.c_str(), &(iter->first)).c_str()) };
				if (relationDeleteClicked) {
					iter = relations.erase(iter);
					if (iter == relations.end()) {
						break;
					}
				}
				else {
					iter++;
				}
			}
		}

	}

public:
	void draw() {
		drawRelationTypes();
		drawBaseRelations();
	}

};

#endif // VN_ENGINE_RELATIONS_CONFIG_H
