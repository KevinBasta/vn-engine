# CMakeList.txt : CMake project for visual_novel_engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project("vn_engine")
set(CMAKE_CXX_STANDARD 20)
add_definitions(-DVN_BASE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\")

add_executable (
	"vn_engine"
	
	# imgui dependancies
	"external/imgui/imgui.cpp"
	"external/imgui/imgui_demo.cpp"
	"external/imgui/imgui_draw.cpp"
	"external/imgui/imgui_tables.cpp"
	"external/imgui/imgui_widgets.cpp"
	"external/imgui/backends/imgui_impl_opengl3.cpp"
	"external/imgui/backends/imgui_impl_glfw.cpp"
	"external/imgui/backends/imgui_impl_opengl3.h"
	"external/imgui/backends/imgui_impl_glfw.h"

	# main program
	"src/vn_engine.cpp"

	# common
	"src/common/timer.cpp"
	"src/common/timer.h"
	"src/common/id.cpp"
	"src/common/id.h"

	# model
	"src/model/base/chapter.cpp"
	"src/model/base/chapter.h"
	"src/model/base/graph.cpp"
	"src/model/base/graph.h"
	"src/model/base/node.cpp"
	"src/model/base/node.h"
	"src/model/base/node_children.cpp"
	"src/model/base/node_children.h"
	
	"src/model/character/character.cpp"
	"src/model/character/character.h"
	"src/model/character/relation_types.cpp" 
	"src/model/character/relation_types.h" 
	"src/model/character/relation.cpp"
	"src/model/character/relation.h"
	
	"src/model/iterators/chapter_iterator.cpp"
	"src/model/iterators/chapter_iterator.h"
	"src/model/iterators/graph_traverser.cpp"
	"src/model/iterators/graph_traverser.h"
	"src/model/iterators/node_runner.cpp"
	"src/model/iterators/node_runner.h"

	"src/model/node/chapter_node.cpp"
	"src/model/node/chapter_node.h"

	"src/model/subjects/model_subject.cpp"
	"src/model/subjects/model_subject.h"
	"src/model/state/state_subject.cpp"
	"src/model/state/state_subject.h"

	"src/model/state/state_types.h"
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
	"src/view/base/texture_manager.h" 
	"src/view/base/texture_manager.cpp" 
	"src/model/builders/chapter_node_builder.h" 
	"src/model/builders/chapter_builder.h" 
	"src/model/builders/character_builder.h" 
	"src/model/builders/relation_builder.h" 
	"src/model/builders/node_builder.h" 
	"src/model/base/node_parents.h" 
	"src/model/base/node_parents.cpp" 
	"src/model/base/node_stray.h" 
	"src/model/base/node_stray.cpp" "src/model/state/state_text.h" "src/model/state/state_background.h" "src/model/state/state_sprites.h" "src/model/stores/texture_store.h"  "src/model/stores/texture_store.cpp")

target_include_directories(
	"vn_engine"
	PUBLIC
	
	"src/"
	"src/common"
	
	"src/model"
	"src/model/base"
	"src/model/character"
	"src/model/iterators"
	"src/model/node"
	"src/model/savefile"
	"src/model/subjects"
	"src/model/builders"
	"src/model/state"
	"src/model/stores"

	"src/view"
	"src/view/base"
	"src/view/engine"
	"src/view/game"
	"src/view/layers"
)

# Avoid warnings/errors from libraries 
target_include_directories(
	"vn_engine"
	SYSTEM
	PUBLIC
	"external/imgui"
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