#ifndef VN_ENGINE_RELATIONS_CONFIG_H
#define VN_ENGINE_RELATIONS_CONFIG_H

#include "state_subject.h"
#include "engine_helpers.h"
#include "model_engine_interface.h"

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

		ModelEngineInterface::Relation& types{ ModelEngineInterface::getRelationTypesMap() };
		
	}

public:
	void draw() {
		drawRelationTypes();
		drawBaseRelations();
	}

};

#endif // VN_ENGINE_RELATIONS_CONFIG_H
