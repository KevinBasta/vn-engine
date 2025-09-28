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
	

	## libraries ## 
	# imgui
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
	
	# sqlite c code (using the cpp wrapper moduel for now to get the c files)
	"external/SQLiteCpp/sqlite3/sqlite3.c"
	"external/SQLiteCpp/sqlite3/sqlite3.h"




	## main program ##
	"src/vn_engine.h"
	"src/vn_engine.cpp"

	## common ##
	# exceptions
	"src/common/exceptions/exceptions.h" 

	# observe
	"src/common/observe/observer.h" 
	"src/common/observe/subject.h" 

	# structs
	"src/common/structs/action_type_mappers.cpp" 
	"src/common/structs/action_type_mappers.h"
	"src/common/structs/action_type_mappers_helpers.h"  
	"src/common/structs/engine_drag_drop_payload.h" 

	# types
	"src/common/types/id.cpp"
	"src/common/types/id.h"
	"src/common/types/index.h"
	
	# util
	"src/common/util/timer.cpp"
	"src/common/util/timer.h" 
	"src/common/util/logging.h" 
	"src/common/util/logging.cpp" 
		 
	## engine ##
	"src/engine/engine.cpp"
	"src/engine/engine.h"
	"src/engine/engine_preview.h"
	"src/engine/engine_helpers.h" 
	"src/engine/engine_helpers.cpp" 

	# node editors
	"src/engine/node_editors/graph_editors_common.cpp"
	"src/engine/node_editors/graph_editors_common.h"

	# graphs
	"src/engine/node_editors/graphs/engine_chapter_graph.h" 
	"src/engine/node_editors/graphs/engine_graph.h" 
	"src/engine/node_editors/graphs/engine_node_graph.h" 

	# timelines
	"src/engine/node_editors/timelines/engine_animations_timeline.h"
	"src/engine/node_editors/timelines/engine_step_timeline.h"
	"src/engine/node_editors/timelines/engine_timeline.h"

	# sidebars
	"src/engine/sidebar/engine_scene_action_type_list.h"
	"src/engine/sidebar/engine_scene_action_editor.cpp"
	"src/engine/sidebar/engine_scene_action_editor.h"
	"src/engine/sidebar/engine_scene_editor.cpp"
	"src/engine/sidebar/engine_scene_editor.h"
	"src/engine/sidebar/engine_relation_editor.h" 
	"src/engine/sidebar/engine_relation_editor.cpp" 
	"src/engine/sidebar/engine_character_editor.h" 
	"src/engine/sidebar/engine_character_editor.cpp" 
	"src/engine/sidebar/engine_texture_editor.h" 
	"src/engine/sidebar/engine_texture_editor.cpp" 
	"src/engine/sidebar/engine_chapter_editor.cpp"
	"src/engine/sidebar/engine_chapter_editor.h" 
	
	## game ##
	"src/game/context.cpp"
	"src/game/context.h"
	"src/game/controller.h"
	"src/game/game_observer.h" 

	# internals
	"src/game/internals/shader.cpp"
	"src/game/internals/shader.h"
	"src/game/internals/text.cpp"
	"src/game/internals/text.h"
	"src/game/internals/texture.cpp"
	"src/game/internals/texture.h"
	"src/game/internals/window.cpp" 
	"src/game/internals/window.h"
	
	# layers
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
	"src/game/layers/layer_main_menu.h" 
	"src/game/layers/layer_main_menu.cpp"  
	"src/game/layers/layer_saves.h" 
	"src/game/layers/layer_saves.cpp" 
	"src/game/layers/layer_options.h" 
	"src/game/layers/layer_options.cpp" 
	"src/game/layers/layer_sidebar.h" 
	"src/game/layers/layer_sidebar.cpp"

	# resource managers
	"src/game/resource_managers/texture_manager.h" 
	"src/game/resource_managers/texture_manager.cpp" 

	## model ##
	"src/model/model_subject.cpp"
	"src/model/model_subject.h"

	# components
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

	# interface
	"src/model/interface/model_common_interface.h"
	"src/model/interface/model_engine_interface.h" 
	"src/model/interface/model_runtime_interface.h"

	# internals
	"src/model/internals/builders/chapter_builder.h"
	"src/model/internals/builders/node_builder.h"
	"src/model/internals/builders/character_builder.h"
	"src/model/internals/builders/linkable_builder.h"
	"src/model/internals/builders/relations_builder.h"
	"src/model/internals/builders/texture_store_builder.h"

	"src/model/internals/linkable/node_types.h"

	"src/model/internals/character/character.cpp"
	"src/model/internals/character/character.h"
	
	"src/model/internals/iterators/chapter_iterator.cpp"
	"src/model/internals/iterators/chapter_iterator.h"
	"src/model/internals/iterators/node_runner.cpp"
	"src/model/internals/iterators/node_runner.h"

	"src/model/internals/linkable/chapter.cpp"
	"src/model/internals/linkable/chapter.h"
	"src/model/internals/linkable/node.cpp"
	"src/model/internals/linkable/node.h"
	"src/model/internals/linkable/linkable.cpp" 
	"src/model/internals/linkable/linkable.h" 
	
	"src/model/internals/relations/relations.cpp"
	"src/model/internals/relations/relations.h"

	"src/model/internals/resource_managers/engine_chapter_manager.h"
	"src/model/internals/resource_managers/engine_node_manager.h"

	"src/model/internals/stores/sound_store.h" 
	"src/model/internals/stores/sound_store.cpp" 
	"src/model/internals/stores/texture_store.h"
	"src/model/internals/stores/texture_store.cpp"
	
	## state ##
	"src/state/state_subject.cpp"
	"src/state/state_subject.h"
	"src/state/state_FSA.h" 
	"src/state/state_FSA.cpp" 

	# components	
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
	"src/state/components/state_main_menu.h" 
	"src/state/components/state_main_menu.cpp" 
	"src/state/components/state_saves.cpp" 
	"src/state/components/state_saves.h" 
	"src/state/components/state_types.h"
)



target_include_directories(
	"vn_engine"
	PUBLIC

	"src"
	"src/common"
	"src/common/exceptions"
	"src/common/observe"
	"src/common/structs"
	"src/common/types"
	"src/common/util"
	
	"src/engine"
	"src/engine/node_editors"
	"src/engine/node_editors/graphs"
	"src/engine/node_editors/timelines"
	"src/engine/sidebar"

	"src/game"
	"src/game/layers"
	"src/game/glsl"
	"src/game/resource_managers"
	"src/game/internals"

	"src/model"
	"src/model/components"
	"src/model/interface"
	"src/model/internals"
	"src/model/internals/builders"
	"src/model/internals/chapter_node"
	"src/model/internals/character"
	"src/model/internals/iterators"
	"src/model/internals/linkable"
	"src/model/internals/relations"
	"src/model/internals/resource_managers"
	"src/model/internals/serialize"
	"src/model/internals/stores"

	"src/state"
	"src/state/components"

	"src/serialize"
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
	"external/SQLiteCpp/sqlite3"
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