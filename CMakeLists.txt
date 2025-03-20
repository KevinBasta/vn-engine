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
	"src/vn_engine.h"
	"src/vn_engine.cpp"
	"src/subjects_orchestrator.h"

	# common
	"src/common/timer.cpp"
	"src/common/timer.h" 
	"src/common/id.cpp"
	"src/common/id.h"
	"src/common/log.h" 
	"src/common/log.cpp" 
	"src/common/index.h"
	#"src/common/types.h"

	"src/common/structs/action_type_mappers.cpp" 
	"src/common/structs/action_type_mappers.h"
	"src/common/structs/action_type_mappers_helpers.h"  
	"src/common/structs/engine_drag_drop_payload.h" 

	 
	# engine
	"src/engine/engine.cpp"
	"src/engine/engine.h"
	"src/engine/engine_observer.h"
	"src/engine/engine_preview.h"
	"src/engine/engine_properties.h" 

	"src/engine/node_editors/graph_editors_common.cpp"
	"src/engine/node_editors/graph_editors_common.h"

	"src/engine/node_editors/graphs/engine_chapter_graph.h" 
	"src/engine/node_editors/graphs/engine_graph.h" 
	"src/engine/node_editors/graphs/engine_node_graph.h" 

	"src/engine/node_editors/timelines/engine_animations_timeline.h"
	"src/engine/node_editors/timelines/engine_step_timeline.h"
	"src/engine/node_editors/timelines/engine_timeline.h"

	"src/engine/sidebar/engine_action_type_list.h"
	"src/engine/sidebar/engine_node_action_fields.cpp"
	"src/engine/sidebar/engine_node_action_fields.h"
	"src/engine/sidebar/engine_node_editor_sidebar.cpp"
	"src/engine/sidebar/engine_node_editor_sidebar.h"

	# game
	"src/game/context.cpp"
	"src/game/context.h"
	"src/game/controller.cpp"
	"src/game/controller.h"
	"src/game/game_loop.h"
	"src/game/game_observer.h" 
	"src/game/observer.h" 

	"src/game/layers/layer_background.cpp"
	"src/game/layers/layer_background.h"
	"src/game/layers/layer_choice.cpp" 
	"src/game/layers/layer_choice.h" 
	"src/game/layers/layer_sprite.cpp"
	"src/game/layers/layer_sprite.h"
	"src/game/layers/layer_text.cpp"
	"src/game/layers/layer_text.h"
	"src/game/layers/layer_transition.h" 
	"src/game/layers/layer_transition.cpp" 

	# game internals
	"src/game_internals/shader.cpp"
	"src/game_internals/shader.h"
	"src/game_internals/text.cpp"
	"src/game_internals/text.h"
	"src/game_internals/texture.cpp"
	"src/game_internals/texture.h"
	"src/game_internals/window.cpp" 
	"src/game_internals/window.h"

	"src/game_internals/resource_managers/texture_manager.h" 
	"src/game_internals/resource_managers/texture_manager.cpp" 

	# model
	"src/model/components/model_chapters.cpp"
	"src/model/components/model_chapters.h"
	"src/model/components/model_characters.cpp" 
	"src/model/components/model_characters.h" 
	"src/model/components/model_relations.h" 
	"src/model/components/model_realtions.cpp"
	"src/model/components/model_relation_types.h" 
	"src/model/components/model_relation_types.cpp"
    "src/model/components/model_textures.cpp" 
    "src/model/components/model_textures.h" 

	"src/model/interface/model_common_interface.h"
	"src/model/interface/model_engine_interface.h" 
	"src/model/interface/model_runtime_interface.h" 
	"src/model/interface/model_subject.cpp"
	"src/model/interface/model_subject.h"

	# model internal
	"src/model_internals/builders/chapter_builder.h"
	"src/model_internals/builders/chapter_node_builder.h"
	"src/model_internals/builders/character_builder.h"
	"src/model_internals/builders/linkable_builder.h"
	"src/model_internals/builders/relations_builder.h"
	"src/model_internals/builders/texture_store_builder.h"

	"src/model_internals/chapter_node/chapter_node.cpp"
	"src/model_internals/chapter_node/chapter_node.h"
	"src/model_internals/chapter_node/chapter_node_types.h"

	"src/model_internals/character/character.cpp"
	"src/model_internals/character/character.h"
	
	"src/model_internals/iterators/chapter_iterator.cpp"
	"src/model_internals/iterators/chapter_iterator.h"
	"src/model_internals/iterators/node_runner.cpp"
	"src/model_internals/iterators/node_runner.h"

	"src/model_internals/linkable/chapter.cpp"
	"src/model_internals/linkable/chapter.h"
	"src/model_internals/linkable/node.cpp"
	"src/model_internals/linkable/node.h"
	"src/model_internals/linkable/linkable.cpp" 
	"src/model_internals/linkable/linkable.h" 
	
	"src/model_internals/relations/relations.cpp"
	"src/model_internals/relations/relations.h"
	 
	 

	"src/model_internals/resource_managers/engine_chapter_manager.h"
	"src/model_internals/resource_managers/engine_node_manager.h"
	"src/model_internals/resource_managers/runtime_chapter_manager.h"

	#"src/model_internals/serialize/json/json_lexer.cpp"
	#"src/model_internals/serialize/json/json_token.cpp"
	#"src/model_internals/serialize/json/json_error.cpp"
	#"src/model_internals/serialize/json/json_expr.cpp"

	"src/model_internals/stores/sound_store.h" 
	"src/model_internals/stores/sound_store.cpp" 
	"src/model_internals/stores/texture_store.h"
	"src/model_internals/stores/texture_store.cpp"
	
	# state
	"src/state/subject.h"
	
	"src/state/components/state_background.cpp" 
	"src/state/components/state_background.h" 
	"src/state/components/state_choices.cpp" 
	"src/state/components/state_choices.h" 
	"src/state/components/state_dialogue.cpp" 
	"src/state/components/state_dialogue.h" 
	"src/state/components/state_next_chapter.cpp" 
	"src/state/components/state_next_chapter.h" 
	"src/state/components/state_next_node.cpp"
	"src/state/components/state_next_node.h"
	"src/state/components/state_relations.cpp" 
	"src/state/components/state_relations.h" 
	"src/state/components/state_sprites.cpp" 
	"src/state/components/state_sprites.h" 

	"src/state/interface/state_common_interface.h" 
	"src/state/interface/state_engine_interface.h" 
	"src/state/interface/state_runtime_interface.h" 
	"src/state/interface/state_subject.cpp"
	"src/state/interface/state_subject.h"
	"src/state/interface/state_types.h"
)

target_include_directories(
	"vn_engine"
	PUBLIC
	
	"src"
	"src/common"
	"src/common/structs"
	
	"src/engine"
	"src/engine/node_editors"
	"src/engine/node_editors/graphs"
	"src/engine/node_editors/timelines"
	"src/engine/sidebar"

	"src/game"
	"src/game/layers"

	"src/game_internals"
	"src/game_internals/glsl"
	"src/game_internals/resource_managers"

	"src/model"
	"src/model/components"
	"src/model/interface"

	"src/model_internals"
	"src/model_internals/builders"
	"src/model_internals/chapter_node"
	"src/model_internals/character"
	"src/model_internals/iterators"
	"src/model_internals/linkable"
	"src/model_internals/relations"
	"src/model_internals/resource_managers"
	"src/model_internals/serialize"
	"src/model_internals/stores"

	"src/state"
	"src/state/components"
	"src/state/interface"
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