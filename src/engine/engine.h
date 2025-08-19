#ifndef VN_ENGINE_GUI_H
#define VN_ENGINE_GUI_H

#include "id.h"

#include "window.h"
#include "context.h"
#include "state_subject.h"

#include "engine_chapter_graph.h"
#include "engine_node_graph.h"
#include "engine_step_timeline.h"
#include "engine_preview.h"
#include "engine_character_editor.h"
#include "engine_texture_editor.h"
#include "engine_relation_editor.h"
#include "engine_chapter_editor.h"
#include "engine_scene_editor.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <GLFW/glfw3.h>

#define WINDOW_VIEWPORT_PREVIEW		"VIEWPORT PREVIEW"
#define WINDOW_CHAPTER_GRAPH		"CHAPTER GRAPH"
#define WINDOW_NODE_GRAPH			"NODE GRAPH"
#define WINDOW_STEP_TIMELINE		"STEP TIMELINE"
#define WINDOW_SCENE_EDITOR			"SCENE EDITOR"
#define WINDOW_CHAPTERS_EDITOR		"CHAPTERS EDITOR"
#define WINDOW_CHARACTERS_CONFIG	"CHARACTERS CONFIG"
#define WINDOW_TEXTURES_CONFIG		"TEXTURES CONFIG"
#define WINDOW_RELATIONS_CONFIG		"RELATIONS CONFIG"
#define WINDOW_IMGUI_DEMO			"Dear ImGui Demo"

static void printTest() {
	std::cout << "imgui test" << std::endl;
}

class VnEngine {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	GameContext* m_context{ nullptr };
	bool m_firstDraw{ true };
	bool m_resetDockspace{ true };
	bool m_expandedDockingView{ false };
	VnEnginePreview m_enginePreview;
	
	VnEngineChapterGraph	m_engineChapterGraph;
	VnEngineNodeGraph		m_engineNodeGraph;
	VnEngineStepTimeline	m_engineStepTimeline;

	VnEngineCharacterEditor m_engineCharacterEditor;
	VnEngineChapterEditor m_engineChapterEditor;
	VnEngineTextureEditor m_engineTextureEditor;
	VnEngineRelationEditor m_engineRelationEditor;
	VnEngineSceneEditor m_engineSceneEditor;


	void initImgui();

public:
	id m_currentChapterId{};
	id m_currentNodeId{};

public:
	VnEngine(VnWindow* window, StateSubject* stateSubject, GameContext* context);
	~VnEngine() {}

	void createDockspace();
	void createMenuBar();
	void draw();
	bool framebufferFocused();
};


static void tabs() {
	/*ImGui::Begin("");
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("TESTETSETSETSETSETSET"))
			{
				ImGui::MenuItem("Main menu bar", NULL, &printTest);

				ImGui::SeparatorText("Mini apps");
				ImGui::MenuItem("Assets Browser", NULL, &printTest);
				ImGui::MenuItem("Console", NULL, &printTest);

				ImGui::SeparatorText("Concepts");
				ImGui::MenuItem("Auto-resizing window", NULL, &printTest);
				ImGui::MenuItem("Auto-resizing window", NULL, &printTest);
				ImGui::MenuItem("Auto-resizing window", NULL, &printTest);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();*/


		// Using a Child allow to fill all the space of the window.
		// It also alows customization
		//ImGui::BeginChild("GameRender");
		// Get the size of the child (i.e. the whole draw size of the windows).
		//ImVec2 wsize = ImGui::GetWindowSize();
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		//ImGui::Image((ImTextureID)tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::EndChild();

		//ImGui::Begin("Graph tree and Node step viewer");

		/*
		static ImVector<int> active_tabs;
		static int next_tab_id = 0;
		if (next_tab_id == 0) // Initialize with some default tabs
			for (int i = 0; i < 3; i++)
				active_tabs.push_back(next_tab_id++);

			// TabItemButton() and Leading/Trailing flags are distinct features which we will demo together.
			// (It is possible to submit regular tabs with Leading/Trailing flags, or TabItemButton tabs without Leading/Trailing flags...
			// but they tend to make more sense together)
			static bool show_leading_button = true;
			static bool show_trailing_button = true;
			ImGui::Checkbox("Show Leading TabItemButton()", &show_leading_button);
			ImGui::Checkbox("Show Trailing TabItemButton()", &show_trailing_button);

			// Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
			static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
			ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", &tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
			if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
				tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
			if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
				tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

			if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
			{
				// Demo a Leading TabItemButton(): click the "?" button to open a menu
				if (show_leading_button)
					if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
						ImGui::OpenPopup("MyHelpMenu");
				if (ImGui::BeginPopup("MyHelpMenu"))
				{
					ImGui::Selectable("Hello!");
					ImGui::EndPopup();
				}

				// Demo Trailing Tabs: click the "+" button to add a new tab.
				// (In your app you may want to use a font icon instead of the "+")
				// We submit it before the regular tabs, but thanks to the ImGuiTabItemFlags_Trailing flag it will always appear at the end.
				if (show_trailing_button)
					if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
						active_tabs.push_back(next_tab_id++); // Add new tab

				// Submit our regular tabs
				for (int n = 0; n < active_tabs.Size; )
				{
					bool open = true;
					char name[16];
					snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
					if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
					{
						ImGui::Text("This is the %s tab!", name);
						ImGui::EndTabItem();
					}

					if (!open)
						active_tabs.erase(active_tabs.Data + n);
					else
						n++;
				}

				ImGui::EndTabBar();
			}
			*/
}

#endif // VN_ENGINE_GUI_H
