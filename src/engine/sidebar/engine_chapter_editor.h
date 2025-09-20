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

	class ChapterProperties {
	private:
		inline static id idToAdd{ 0 };

		static bool chapterExists(id chapterId) {
			return ModelCommonInterface::getChapterById(chapterId) != nullptr;
		}

		static bool drawLinkedNodesGrouping(Chapter* chapter, std::set<id>& list, bool isParentOf) {
			bool modified{ false };

			bool isTreeOpen{ false };

			if (isParentOf) {
				isTreeOpen = ImGui::TreeNodeEx(addIdFromPtr("Chapter Children###stayopen", &list).c_str(), ImGuiTreeNodeFlags_Framed);
			}
			else {
				isTreeOpen = ImGui::TreeNodeEx(addIdFromPtr("Chapter Parents###stayopen", &list).c_str(), ImGuiTreeNodeFlags_Framed);
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ADD_TO_SET"))
				{
					assert(payload->DataSize == sizeof(id));
					id payloadCast = *(const id*)payload->Data;

					if (payloadCast > 0 && chapterExists(payloadCast)) {
						if (isParentOf) {
							ChapterBuilder{ chapter }.link(ModelCommonInterface::getChapterById(payloadCast));
						}
						else {
							ChapterBuilder{ ModelCommonInterface::getChapterById(payloadCast) }.link(chapter);
						}
					}
					else {
						EngineToolTip::setTooltipFor(500, "Invalid Chapter Id!");
					}
				}

				modified = true;

				ImGui::EndDragDropTarget();
			}

			if (isTreeOpen) {
				int i{ 0 };
				for (auto iter{ list.begin() }; iter != list.end(); iter++) {
					ImGui::Text((std::string("#") + std::to_string(i)).c_str());
					ImGui::SameLine();
					ImGui::PushItemWidth(100.0f);

					id dummyInt{ *iter };
					ImGui::DragInt(addIdFromPtr("Value", &(*iter)).c_str(), &dummyInt, 0.0f, *iter, *iter, "%d", 0);

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						std::string idAsStr{ std::to_string(*iter) };

						if (isParentOf) {
							ImGui::SetDragDropPayload("DELETE_CHILD", &(*iter), sizeof(id));
							ImGui::Text((std::string("child node ") + idAsStr).c_str());
						}
						else {
							ImGui::SetDragDropPayload("DELETE_PARENT", &(*iter), sizeof(id));
							ImGui::Text((std::string("parent node ") + idAsStr).c_str());
						}

						ImGui::EndDragDropSource();
					}

					ImGui::PopItemWidth();

					i++;
				}



				ImGui::TreePop();
			}

			return modified;
		}


	public:
		static void draw(id chapterId) {
			ImGui::Spacing();

			Chapter* chapter{ ModelEngineInterface::getChapterById(chapterId) };
			if (chapter == nullptr) { return; }

			bool isTreeOpen = ImGui::TreeNodeEx("Parents and Children###stayopen", ImGuiTreeNodeFlags_Framed);

			if (isTreeOpen) {
				// Add/remove node parents
				bool parentsModified = drawLinkedNodesGrouping(chapter, LinkableBuilder{ chapter }.getParents(), false);

				ImGui::Spacing();

				// Add/remove node children
				bool childrenModified = drawLinkedNodesGrouping(chapter, LinkableBuilder{ chapter }.getChildren(), true);

				// Delete button for parents or children
				bool deleteClicked = ImGui::Button("Delete##DeleteAParentOrChildNodeLink", ImVec2(260.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DELETE_PARENT"))
					{
						assert(payload->DataSize == sizeof(id));
						id payloadCast = *(const id*)payload->Data;

						ChapterBuilder{ ModelCommonInterface::getChapterById(payloadCast) }.unlink(chapter);
					}
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DELETE_CHILD"))
					{
						assert(payload->DataSize == sizeof(id));
						id payloadCast = *(const id*)payload->Data;

						ChapterBuilder{ chapter }.unlink(ModelCommonInterface::getChapterById(payloadCast));
					}

					ImGui::EndDragDropTarget();
				}

				if (deleteClicked) {
					EngineToolTip::setTooltipFor(500, "Drag parent/child here to delete a link!");
				}

				// Add a new node as parent or child
				ImGui::Text("Drag To Add Chapter Id: ");
				ImGui::SameLine();

				ImGui::PushItemWidth(129.0f);
				ImGui::DragInt(addIdFromPtr("####", &(idToAdd)).c_str(), &idToAdd, 0.0f, idToAdd, idToAdd, "%d", 0);
				ImGui::PopItemWidth();

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("ADD_TO_SET", &(idToAdd), sizeof(id));
					ImGui::Text((std::string("Adding ") + std::to_string(idToAdd)).c_str());

					ImGui::EndDragDropSource();
				}

				ImGui::TreePop();
			}
		}
	};

public:
	void draw() {
		ModelEngineInterface::ChapterMap& chapters{ ModelEngineInterface::getChaptersMap() };

		ImGui::SeparatorText("Starting Chapter: ");
		static id startingChapter{ ModelEngineInterface::getFirstChapterId() };
		bool startingChapterModified{ ImGui::DragInt("##EngineStartingChapterIdSetter", &startingChapter, 1, 1, INT_MAX)};
		if (startingChapterModified) {
			ModelEngineInterface::setFirstChapterId(startingChapter);
		}

		ImGui::SeparatorText("Add New Chapter");

		// Chatper title
		ImGui::Text("Chapter Title: ");
		ImGui::SameLine();
		static std::string newChapterName{};
		ImGui::PushItemWidth(250.0f);
		ImGui::InputText(addIdFromPtr("##chapterAddName", &newChapterName).c_str(), &(newChapterName), 0);

		// Chapter head node id
		ImGui::Text("Head Node Id: ");
		ImGui::SameLine();
		static int tempHeadNodeId{ 0 };
		bool tempHeadNodeIdModified{ ImGui::DragInt(addIdFromPtr("##addheadnodeid", &tempHeadNodeId).c_str(), &tempHeadNodeId, 1, 1, INT_MAX) };
		if (tempHeadNodeIdModified) { std::cout << "temp head node id modified" << std::endl; }
		ImGui::PopItemWidth();

		// Add button
		bool addClicked = ImGui::Button("Add##addNewChapterStatic", ImVec2(70.0f, 0.0f));
		if (addClicked) {
			ChapterBuilder builder{};
			builder.setName(myconv.from_bytes(newChapterName));
			builder.setHeadNodeId(tempHeadNodeId);
			ModelEngineInterface::addChapter(builder.get());
			for (auto& [i, j] : chapters) {
				std::cout << i << std::endl;
			}
		}

		ImGui::SeparatorText("Modify Existing Chapter");
		
		ImGui::PushItemWidth(200);
		static id currentChapterId{ chapters.begin()->first };
		if (chapters.size() > 0) {
			if (currentChapterId == 0) {
				currentChapterId = chapters.begin()->first;
			}
		}
		else {
			return;
		}

		std::string currentChapterName{ myconv.to_bytes(chapters.at(currentChapterId).get()->getName()) };

		if (ImGui::BeginCombo(addIdFromPtr("###chapterpickerforviewmodify", &(currentChapterId)).c_str(), currentChapterName.c_str(), 0)) {
			for (auto charactersIter{ chapters.begin() }; charactersIter != chapters.end(); charactersIter++) {
				const bool isSelected = (currentChapterId == charactersIter->first);

				std::string curCharacterName{ myconv.to_bytes(charactersIter->second.get()->getName()) };
				if (ImGui::Selectable(addIdFromPtr(curCharacterName, &charactersIter->first).c_str(), isSelected)) {
					currentChapterId = charactersIter->first;
				}

			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		// Modify chapter title
		ImGui::Text("Chapter Id: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(currentChapterId).c_str());

		ImGui::Text("Chapter Title: ");
		ImGui::SameLine();
		std::string convertedName = myconv.to_bytes(chapters[currentChapterId].get()->getName());
		ImGui::PushItemWidth(250.0f);
		bool nameModified{ ImGui::InputText(addIdFromPtr("##chapterName", &currentChapterId).c_str(), &(convertedName), 0) };
		if (nameModified) { ChapterBuilder{ chapters[currentChapterId].get() }.setName(myconv.from_bytes(convertedName)); }

		// Modify chapter head node id
		ImGui::Text("Head Node Id: ");
		ImGui::SameLine();
		int currentHeadNodeId{ chapters[currentChapterId].get()->getHeadNodeId() };
		bool headNodeIdModified{ ImGui::DragInt(addIdFromPtr("###headnodeidcurrent", &currentHeadNodeId).c_str(), &currentHeadNodeId, 1, 1, INT_MAX)};
		ImGui::PopItemWidth();
		if (headNodeIdModified) {
			ChapterBuilder{ chapters[currentChapterId].get() }.setHeadNodeId(currentHeadNodeId);
		}

		// Modify parents and children
		ChapterProperties::draw(currentChapterId);

		// Delete button
		bool deleteClicked = ImGui::Button(addIdFromPtr("Delete##delChapter", &(currentHeadNodeId)).c_str(), ImVec2(70.0f, 0.0f));
		if (deleteClicked) {
			chapters.erase(currentChapterId);
			if (chapters.begin() == chapters.end()) {
				currentChapterId = 0;
			}
			else {
				currentChapterId = chapters.begin()->first;
			}
		}

	}

};


#endif // ENGINE_CHAPTER_CONFIG_EDITOR_H