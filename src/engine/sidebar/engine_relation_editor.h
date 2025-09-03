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
		
		for (auto iter{ relations.begin() }; iter != relations.end(); iter++) {
			Character* mainCharacterOfRelation{ ModelEngineInterface::getCharacterById(iter->first) };
			if (mainCharacterOfRelation == nullptr) {
				continue;
			}

			ImGui::Separator();
			std::string title{ "Relations of " + myconv.to_bytes(mainCharacterOfRelation->getName()) };
			ImGui::Text(title.c_str());
			ImGui::SameLine();
			bool relationDeleteClicked{ ImGui::Button(addIdFromPtr("Delete###", &(iter->first)).c_str()) };
			if (relationDeleteClicked) {
				iter = relations.erase(iter);
				if (iter == relations.end()) {
					break;
				}
			}

			Relations* curRelations{ iter->second.get() };

			if (curRelations != nullptr) {
				auto characterToRelationMap{ RelationsBuilder{curRelations}.getCharacterToRelationMap() };

				for (auto iter2{ characterToRelationMap.begin() }; iter2 != characterToRelationMap.end(); iter2++) {
					Character* relationWithCharacter{ ModelEngineInterface::getCharacterById(iter2->first) };
					if (relationWithCharacter == nullptr) {
						continue;
					}

					std::string subtitle{ "Relation with: " + myconv.to_bytes(relationWithCharacter->getName()) };
					ImGui::Text(subtitle.c_str());
					ImGui::SameLine();
					bool relationWithDeleteClicked{ ImGui::Button(addIdFromPtr("Delete###", &(iter2->first)).c_str()) };
					if (relationWithDeleteClicked) {
						iter2 = characterToRelationMap.erase(iter2);
						if (iter2 == characterToRelationMap.end()) {
							break;
						}
					}

					for (auto iter3{ iter2->second.begin() }; iter3 != iter2->second.end(); iter3++) {
						auto relationTypeIter{ relationTypes.find(iter3->first) };
						if (relationTypeIter != relationTypes.end()) {
							ImGui::Text(relationTypeIter->second.c_str());
							ImGui::SameLine();
							int curVal{ iter3->second };
							bool curValModified{ ImGui::DragInt(addIdFromPtr("###", &(iter3->second)).c_str(), &(curVal), 1.0f, INT_MIN, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp) };
							if (curValModified) { iter2->second[iter3->first] = curVal; break; }
							ImGui::SameLine();
						}
						
						bool relationTypeDeleteClicked{ ImGui::Button(addIdFromPtr("Delete###", &(iter3->first)).c_str()) };
						if (relationTypeDeleteClicked) {
							iter3 = iter2->second.erase(iter3);
							if (iter3 == iter2->second.end()) {
								break;
							}
						}
					}
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
