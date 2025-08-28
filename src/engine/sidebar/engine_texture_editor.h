#ifndef VN_ENGINE_TEXTURES_CONFIG_H
#define VN_ENGINE_TEXTURES_CONFIG_H

#include "state_subject.h"

#include "texture.h"
#include "engine_helpers.h"
#include "model_engine_interface.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"


#include <GLFW/glfw3.h>

#include <unordered_map>
#include <memory>
#include <codecvt>
#include <iostream>

class VnEngineTextureEditor {
private:
	static StateSubject* m_stateSubject;
	static index m_selectedComboId;
	static std::string addComboId(std::string str) { return (std::string(str) + std::string("##TextureStoreComboChoiceId")); }

public:
	VnEngineTextureEditor(StateSubject* stateSubject)
	{
		m_stateSubject = stateSubject;
	}

	~VnEngineTextureEditor() {}

private:
	inline std::string getName(ModelEngineInterface::TextureStoreMap& map, id id) {
		auto iter{ map.find(id) };
		
		if (iter != map.end()) {
			return iter->second.get()->getName();
		}

		return "";
	}

	void drawStoresCombo(ModelEngineInterface::TextureStoreMap& map) {
		//std::unordered_map<id, std::unique_ptr<TextureStore>>;
		if (ImGui::BeginCombo("##TextureStoreCombo", addComboId(getName(map, m_selectedComboId)).c_str(), NULL))
		{
			for (auto& [storeId, store] : map) {
				const bool isSelected = (storeId == m_selectedComboId);
				if (ImGui::Selectable(addComboId(getName(map, storeId)).c_str(), isSelected)) {
					m_selectedComboId = storeId;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}
	}

	void addNewStore(ModelEngineInterface::TextureStoreMap& map) {
		static std::string storeName{};

		ImGui::Text("Store Name: ");
		ImGui::SameLine();
		ImGui::InputText(addIdFromPtr("##theCharacterName", &storeName).c_str(), &storeName);
		
		bool addClicked = ImGui::Button("Add##AddTextureStoreToGame", ImVec2(260.0f, 0.0f));
		if (addClicked) {
			std::unique_ptr<TextureStore> allocatedTextureStore{ std::make_unique<TextureStore>(storeName) };

			map.insert({ allocatedTextureStore.get()->getId(), std::move(allocatedTextureStore) });
		}
	}

	void drawStore(ModelEngineInterface::TextureStoreMap& map) {
		auto iter{ map.find(m_selectedComboId) };
		if (iter == map.end()) {
			return;
		}

		TextureStore* store{ iter->second.get() };

		std::string storeName{ store->getName() };
		bool nameModified{ ImGui::InputText(addIdFromPtr("Texture Store Name##TextureStoreName", store).c_str(), &storeName) };
		if (nameModified) { TextureStoreBuilder{ store }.setName(storeName); }
		
		auto& storeVector{ TextureStoreBuilder{ store }.getTexturePathVector() };
		int i{ 0 };
		for (std::optional<std::string>& elem : storeVector) {
			if (!elem.has_value()) { elem.emplace(""); }
			
			std::string title{ std::string("#") + std::to_string(i) + std::string(" Texture Path ") };

			ImGui::Text(title.c_str());
			bool pathModified{ ImGui::InputText(addIdFromPtr("##TextureStoreName", &elem).c_str(), &(elem.value())) };

			if (pathModified) {
				store->forceReloadTexture(i);
			}

			ImGui::Text("Texture Preview");
			Texture2D* texture{ TextureManager::getTexture({store->getId(), i})};
			if (texture == nullptr) {
				store->forceReloadTexture(i);
			}
			else {
				try {
					ImGui::Image((ImTextureID)(intptr_t)texture->openglId(), ImVec2(texture->width() / 5, texture->height() / 5));
				}
				catch (...) {
					// do nothing
				}
			}

			bool deleteTexture = ImGui::Button(addIdFromPtr("Delete##TextureFromStore", &elem).c_str(), ImVec2(150, 25));
			if (deleteTexture) {
				// TODO: maybe allow the last index in the vector to actually be deleted
				elem.value() = "";
				TextureStoreBuilder{ store }.removeTexture(i);
			}

			ImGui::Spacing();
			i++;
		}
		
		ImGui::Text("Add New Texture");
		static std::string newTexturePath{ "" };
		ImGui::InputText(addIdFromPtr("##TextureStoreName", &newTexturePath).c_str(), &(newTexturePath));

		bool staticPathModified{ ImGui::Button(addIdFromPtr("Add Texture", &newTexturePath).c_str(), ImVec2(150, 25)) };
		if (staticPathModified) {
			bool added = TextureStoreBuilder{ store }.addTexture(newTexturePath);
			
			if (added) {
				newTexturePath = "";
			}
			else {
				EngineToolTip::setTooltipFor(500, "Invalid Texture Path!");
			}
		}

		ImGui::Spacing();

		bool deleteStore = ImGui::Button(addIdFromPtr("Delete Store##", store).c_str(), ImVec2(150, 25));
		if (deleteStore) {
			map.erase(iter);
		}
	}

public:

	void draw() {
		ModelEngineInterface::TextureStoreMap& map{ ModelEngineInterface::getTextureStoreMap() };
		ImGui::SeparatorText("Add Texture Store");
		addNewStore(map);

		ImGui::SeparatorText("Current Texture Stores");
		drawStoresCombo(map);

		drawStore(map);
	}

};

#endif // VN_ENGINE_TEXTURES_CONFIG_H
