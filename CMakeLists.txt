# CMakeList.txt : CMake project for visual_novel_engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project("vn_engine")
set(CMAKE_CXX_STANDARD 20)

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
	"vn_engine.cpp"

	# common
	"src/common/timer.cpp"
	"src/common/id.cpp"

	# model
	"src/model/subject.h"
	"src/model/state_subject.h"
	"src/model/model_subject.h"
	"src/model/character/character.cpp"
	"src/model/character/relation_types.cpp" 
	"src/model/character/relation.cpp"
	
	"src/model/story/chapter.cpp"
	"src/model/story/graph.cpp"
	"src/model/story/node.cpp"
	"src/model/story/node_children.cpp" 
	
	"src/model/savefile/json_lexer.cpp"
	"src/model/savefile/json_token.cpp"
	"src/model/savefile/json_error.cpp"
	"src/model/savefile/json_expr.cpp"

	# runtime
	"src/runtime/runtime_characters.cpp"
	"src/runtime/runtime_relations.cpp"
	"src/runtime/runtime_chapters.cpp" 

	# view
	"src/view/observer.h" 
	"src/view/game_observer.h" 
	"src/view/engine_observer.h" 
	"src/view/common/shader.cpp" 
	"src/view/common/texture.cpp"

	"src/view/engine/engine.cpp"

	"src/view/game/camera.cpp" 
	"src/view/game/frame.cpp" 
	
 "src/view/game/context.cpp" "src/view/game/context.h" "src/view/game/controller.cpp" "src/view/game/controller.h" "src/model/story/chapter_node.cpp" "src/model/story/chapter_node.h" "src/view/game/window.cpp" "src/view/game/window.h" "src/view/gameloop.h")

target_include_directories(
	"vn_engine"
	PUBLIC
	""
	"src/common"
	"src/model"
	"src/model/character"
	"src/model/savefile"
	"src/model/story"
	"src/runtime"
	"src/view"
	"src/view/common"
	"src/view/engine"
	"src/view/game"
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
)

# Add libraries
add_subdirectory("external/glad")
add_subdirectory("external/glfw")
add_subdirectory("external/glm")
target_link_libraries("vn_engine" PRIVATE glad glfw glm)

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