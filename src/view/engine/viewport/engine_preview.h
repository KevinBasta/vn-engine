#ifndef VN_ENGINE_PREVIEW_H
#define VN_ENGINE_PREVIEW_H

#include "window.h"
#include "context.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <GLFW/glfw3.h>

class VnEnginePreview {
private:
	GameContext* m_context{ nullptr };

public:
	VnEnginePreview(GameContext* context) :
		m_context{ context }
	{

	}

	void draw() {
		//
		// Game view window
		//
		const float newWidth = ImGui::GetContentRegionAvail().x;
		const float newHeight = ImGui::GetContentRegionAvail().y;

		FrameDimensions windowSizeUpdate{ getNormalizedDimentions(newWidth, newHeight) };
		glViewport(0, 0, windowSizeUpdate.width, windowSizeUpdate.height);

		unsigned int textureId = m_context->getTextureId();
		ImVec2 pos = ImGui::GetCursorScreenPos(); // screen position of the window

		// and here we can add our created texture as image to ImGui
		ImGui::GetWindowDrawList()->AddImage(
			(void*)textureId,
			ImVec2(pos.x, pos.y),
			ImVec2(pos.x + windowSizeUpdate.width, pos.y + windowSizeUpdate.height),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);

		m_context->renderEngine(windowSizeUpdate);

		/*if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &printTest);
				ImGui::MenuItem("Padding", NULL, &printTest);
				ImGui::Separator();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}*/
	}

};


#endif // VN_ENGINE_PREVIEW_H