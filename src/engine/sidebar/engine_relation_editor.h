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
		ImGui::SeparatorText("Base Relations");

		ModelEngineInterface::RelationsMap& relations{ ModelEngineInterface::getRelationsMap() };
		ModelEngineInterface::RelationTypeMap& relationTypes{ ModelEngineInterface::getRelationTypesMap() };
		ModelEngineInterface::CharacterMap& characterMap{ ModelEngineInterface::getCharacterMap() };
		
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
				bool relationDeleteClicked{ ImGui::Button(addIdFromPtr("Delete###relationOf", &(iter->first)).c_str()) };

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
					std::string subtitle{ "Relation with: " + myconv.to_bytes(relationWithCharacter->getName()) };
					ImGui::Text(subtitle.c_str());
					ImGui::SameLine();
					bool relationWithDeleteClicked{ ImGui::Button(addIdFromPtr("Delete###relationWith", &(iter2->first)).c_str()) };

					auto& relationshipFeelings{ iter2->second };

					// Iterate through the differnet types of feelings in the relation
					for (auto iter3{ relationshipFeelings.begin() }; iter3 != relationshipFeelings.end();) {
						// Get the name of the relation feeling to display it
						auto relationTypeIter{ relationTypes.find(iter3->first) };
						if (relationTypeIter != relationTypes.end()) {
							ImGui::Text(relationTypeIter->second.c_str());
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
					// Add a relation feeling type to the character relationship with another character obj
					ImGui::Text("Add Relation Type: ");
					ImGui::SameLine();
					ImGui::PushItemWidth(200);
					static id relationTypeToAdd{ relationTypes.begin()->first }; // TODO change to non static 
					if (ImGui::BeginCombo(addIdFromPtr("###RelationTypePickerForCharacter", &(iter2->first)).c_str(), relationTypes.at(relationTypeToAdd).c_str(), 0)) {
						for (auto relationTypeIter{ relationTypes.begin() }; relationTypeIter != relationTypes.end(); relationTypeIter++) {
							const bool isSelected = (relationTypeToAdd == relationTypeIter->first);

							if (ImGui::Selectable(relationTypeIter->second.c_str(), isSelected)) {
								relationTypeToAdd = relationTypeIter->first;
							}

						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();
					ImGui::SameLine();
					bool relationTypeAddClicked{ ImGui::Button(addIdFromPtr("Add###relationItself", &(iter2->first)).c_str(), {50, 20}) };
					if (relationTypeAddClicked) {
						if (relationshipFeelings.find(relationTypeToAdd) == relationshipFeelings.end()) {
							relationshipFeelings.insert({ relationTypeToAdd, 0 });
						}
						else {
							EngineToolTip::setTooltipFor(500, "Relation Type Already Used!");
						}
					}

					// Allow delete of relation with a character
					if (relationWithDeleteClicked) {
						iter2 = characterToRelationMap.erase(iter2);
						if (iter2 == characterToRelationMap.end()) {
							break;
						}
					}
					else {
						iter2++;
					}
				}

				// Add a relation with a character
				ImGui::Text("Add Relation With: ");
				ImGui::SameLine();
				ImGui::PushItemWidth(200);
				static id characterToAddRelationWtih{ relationTypes.begin()->first }; // TODO change to non static 
				std::string curCharacterName{ myconv.to_bytes(characterMap.at(characterToAddRelationWtih).get()->getName()) };

				if (ImGui::BeginCombo(addIdFromPtr("###RelationTypePickerForCharacter", &(iter->first)).c_str(), curCharacterName.c_str(), 0)) {
					for (auto charactersIter{ characterMap.begin() }; charactersIter != characterMap.end(); charactersIter++) {
						const bool isSelected = (characterToAddRelationWtih == charactersIter->first);

						std::string curCharacterName{ myconv.to_bytes(charactersIter->second.get()->getName()) };
						if (ImGui::Selectable(curCharacterName.c_str(), isSelected)) {
							characterToAddRelationWtih = charactersIter->first;
						}

					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				bool relationWithAddClicked{ ImGui::Button(addIdFromPtr("Add###relationWith", &(iter->first)).c_str(), {50, 20}) };
				if (relationWithAddClicked) {
					if (characterToRelationMap.find(characterToAddRelationWtih) == characterToRelationMap.end()) {
						characterToRelationMap.insert({ characterToAddRelationWtih, {} });
					}
					else {
						EngineToolTip::setTooltipFor(500, "Relation With Character Exists!");
					}
				}


				// Allow delete of relation of a character
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

		// Add relations of a character

		bool relationAddClicked{ ImGui::Button(addIdFromPtr("Add###relationOf", &(relations)).c_str(), {150, 50})};


	}

public:
	void draw() {
		drawRelationTypes();
		drawBaseRelations();
	}

};

#endif // VN_ENGINE_RELATIONS_CONFIG_H
