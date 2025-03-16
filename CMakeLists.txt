# CMakeList.txt : CMake project for visual_novel_engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project("vn_engine")
set(CMAKE_CXX_STANDARD 20)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions(-DVN_BASE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\")

add_executable (
	"vn_engine"
	
	# imgui dependancies
	"external/imgui/imgui_tables.cpp"
	"external/imgui/imgui_widgets.cpp"
	"external/imgui/imgui_draw.cpp"
	"external/imgui/imgui_demo.cpp"
	"external/imgui/imconfig.h"
	"external/imgui/imgui.cpp"
	"external/imgui/imgui.h"
	"external/imgui/backends/imgui_impl_opengl3.cpp"
	"external/imgui/backends/imgui_impl_opengl3.h"
	"external/imgui/backends/imgui_impl_glfw.cpp"
	"external/imgui/backends/imgui_impl_glfw.h"
	"external/imgui/misc/cpp/imgui_stdlib.cpp"
	"external/imgui/misc/cpp/imgui_stdlib.h"

	# imgui node editor
	"external/imgui-node-editor/imgui_node_editor.cpp"
	"external/imgui-node-editor/imgui_canvas.cpp"
	"external/imgui-node-editor/imgui_node_editor_api.cpp"
	"external/imgui-node-editor/crude_json.cpp"

	# main program
	"src/vn_engine.cpp"

	# common
	"src/common/timer.cpp"
	"src/common/timer.h"
	"src/common/id.cpp"
	"src/common/id.h"
	"src/common/log.h" 
	"src/common/log.cpp" 
	"src/common/index.h"

	# model
	"src/model/builders/chapter_builder.h"
	"src/model/builders/chapter_node_builder.h"
	"src/model/builders/character_builder.h"
	"src/model/builders/linkable_builder.h"
	"src/model/builders/relations_builder.h"
	"src/model/builders/texture_store_builder.h"

	"src/model/chapter_node/chapter_node.cpp"
	"src/model/chapter_node/chapter_node.h"
	"src/model/chapter_node/chapter_node_types.h"

	"src/model/character/character.cpp"
	"src/model/character/character.h"
	"src/model/character/relations.cpp"
	"src/model/character/relations.h"
	"src/model/character/relation_types.cpp" 
	"src/model/character/relation_types.h" 
	
	"src/model/iterators/chapter_iterator.cpp"
	"src/model/iterators/chapter_iterator.h"
	"src/model/iterators/node_runner.cpp"
	"src/model/iterators/node_runner.h"

	"src/model/linkable/chapter.cpp"
	"src/model/linkable/chapter.h"
	"src/model/linkable/node.cpp"
	"src/model/linkable/node.h"
	"src/model/linkable/linkable.cpp" 
	"src/model/linkable/linkable.h" 
	
	"src/model/stores/texture_store.h"
	"src/model/stores/texture_store.cpp"
	
	"src/model/subject_model/components/model_chapters.cpp"
	"src/model/subject_model/components/model_chapters.h"
	"src/model/subject_model/components/model_characters.cpp" 
	"src/model/subject_model/components/model_characters.h" 
	"src/model/subject_model/components/model_relations.h" 
	"src/model/subject_model/components/model_realtions.cpp" 
    "src/model/subject_model/components/model_textures.cpp" 
    "src/model/subject_model/components/model_textures.h" 
	"src/model/subject_model/interface/model_subject.cpp"
	"src/model/subject_model/interface/model_subject.h"
	"src/model/subject_model/interface/model_runtime_interface.h" 
	"src/model/subject_model/interface/model_engine_interface.h" 
	"src/model/subject_model/interface/model_common_interface.h"

	"src/model/subject_state/components/state_background.h" 
	"src/model/subject_state/components/state_choices.cpp" 
	"src/model/subject_state/components/state_choices.h" 
	"src/model/subject_state/components/state_dialogue.h" 
	"src/model/subject_state/components/state_next_chapter.h" 
	"src/model/subject_state/components/state_next_node.h"
	"src/model/subject_state/components/state_relations.cpp" 
	"src/model/subject_state/components/state_relations.h" 
	"src/model/subject_state/components/state_sprites.cpp" 
	"src/model/subject_state/components/state_sprites.h" 
	"src/model/subject_state/interface/state_subject.cpp"
	"src/model/subject_state/interface/state_subject.h"
	"src/model/subject_state/interface/state_types.h"
	
	"src/model/subject.h"
	"src/model/subjects_orchestrator.h"

	"src/model/savefile/json/json_lexer.cpp"
	"src/model/savefile/json/json_token.cpp"
	"src/model/savefile/json/json_error.cpp"
	"src/model/savefile/json/json_expr.cpp"

	# view
	"src/view/base/window.cpp" 
	"src/view/base/window.h"
	"src/view/base/texture.cpp"
	"src/view/base/texture.h"
	"src/view/base/text.cpp"
	"src/view/base/text.h"
	"src/view/base/shader.cpp"
	"src/view/base/shader.h"
	
	"src/view/engine/engine.cpp"
	"src/view/engine/engine.h"

	"src/view/game/context.cpp"
	"src/view/game/context.h"
	"src/view/game/controller.cpp"
	"src/view/game/controller.h"

	"src/view/layers/layer_background.cpp"
	"src/view/layers/layer_background.h"
	"src/view/layers/layer_sprite.cpp"
	"src/view/layers/layer_sprite.h"
	"src/view/layers/layer_text.cpp"
	"src/view/layers/layer_text.h"

	"src/view/observer.h" 
	"src/view/game_observer.h" 
	"src/view/engine_observer.h" 
	"src/view/game_loop.h"
 
	"src/view/layers/layer_choice.cpp" 
	"src/view/layers/layer_choice.h" 
	"src/view/layers/layer_transition.h" 
	"src/view/layers/layer_transition.cpp" 
	"src/view/resource_managers/texture_manager.h" 
	"src/view/resource_managers/texture_manager.cpp" 
	"src/view/engine_properties.h" 
	"src/view/engine/node_editors/graphs/engine_graph.h" 
	"src/view/engine/viewport/engine_preview.h" 
	"src/view/engine/viewport/engine_preview.cpp" 
	"src/model/resource_managers/runtime_chapter_manager.h" 
	"src/model/resource_managers/engine_chapter_manager.h" 
	"src/model/resource_managers/engine_node_manager.h" 
	"src/view/engine/node_editors/graphs/engine_chapter_graph.h" 
	"src/view/engine/node_editors/graphs/engine_node_graph.h" 
	 
	"src/model/stores/sound_store.h" 
	"src/model/stores/sound_store.cpp" 
 "src/view/engine/sidebar/engine_node_editor_sidebar.cpp"
 "src/view/engine/sidebar/engine_node_action_fields.h"
 "src/view/engine/sidebar/engine_node_action_fields.cpp" 
 "src/common/structs/action_type_mappers.h"
 "src/common/structs/action_type_mappers_helpers.h"  
 "src/view/engine/node_editors/timelines/engine_timeline.h"
 "src/view/engine/node_editors/timelines/engine_step_timeline.h" 
 "src/view/engine/node_editors/timelines/engine_animations_timeline.h" 
 "src/view/engine/node_editors/graph_editors_common.h" 
 "src/view/engine/node_editors/graph_editors_common.cpp"
 "src/model/subject_state/components/state_background.cpp" "src/model/subject_state/components/state_dialogue.cpp" "src/model/subject_state/components/state_next_chapter.cpp" "src/model/subject_state/components/state_next_node.cpp" "src/model/subject_state/interface/state_common_interface.h" "src/model/subject_state/interface/state_engine_interface.h" "src/model/subject_state/interface/state_runtime_interface.h" "test/old_files/temp.cpp"  "src/view/engine/sidebar/engine_action_type_list.h" "src/common/structs/engine_drag_drop_payload.h" "src/common/structs/action_type_mappers.cpp")

target_include_directories(
	"vn_engine"
	PUBLIC
	
	"src/"
	"src/common"
	"src/common/structs"
	
	"src/model"
	"src/model/builders"
	"src/model/chapter_node"
	"src/model/character"
	"src/model/iterators"
	"src/model/linkable"
	"src/model/savefile"
	"src/model/stores"
	"src/model/subject_model"
	"src/model/subject_model/components"
	"src/model/subject_model/interface"
	"src/model/subject_state"
	"src/model/subject_state/components"
	"src/model/subject_state/interface"
	"src/model/resource_managers"

	"src/view"
	"src/view/base"
	"src/view/engine"
	"src/view/engine/graphs"
	"src/view/engine/sidebar"
	"src/view/engine/viewport"
	"src/view/engine/node_editors"
	"src/view/engine/node_editors/graphs"
	"src/view/engine/node_editors/timelines"
	"src/view/game"
	"src/view/layers"
	"src/view/resource_managers"
)

# Avoid warnings/errors from libraries 
target_include_directories(
	"vn_engine"
	SYSTEM
	PUBLIC
	"external/imgui"
	"external/imgui/misc/cpp"
	"external/imgui/backends"
	"external/imgui-node-editor"
	"external/stb"
	"external/glad"
	"external/glfw"
	"external/glm"
	"external/freetype"
)

# Add libraries
add_subdirectory("external/glad")
add_subdirectory("external/glfw")
add_subdirectory("external/glm")
add_subdirectory("external/freetype")
target_link_libraries("vn_engine" PRIVATE glad glfw glm freetype)

#project ("vn_runtime")


#project ("local_test")

#add_executable (
#	"local_test"
#	"local_test/local_test.cpp"
#)

#target_include_directories(
#	"local_test"
#	PUBLIC
#	"local_test"
#)