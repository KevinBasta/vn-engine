# CMakeList.txt : CMake project for visual_novel_engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project("vn_engine")
set(CMAKE_CXX_STANDARD 20)

add_executable (
	"vn_engine"
	
	"vn_engine.cpp"

	"src/savefile/json_lexer.cpp"
	"src/savefile/json_token.cpp"
	"src/savefile/json_error.cpp"
	"src/savefile/json_expr.cpp"

	"src/core/chapter.cpp"
	"src/core/character.cpp"
	"src/core/relation_types.cpp" 
	"src/core/relation.cpp"
	"src/core/graph.cpp"
	"src/core/node.cpp"
	"src/core/node_children.cpp" 
	"src/core/id.cpp"
	
	"src/runtime/runtime_characters.cpp"
	"src/runtime/runtime_relations.cpp"
	"src/runtime/runtime_chapters.cpp" 
	
	"src/utility/timer.cpp"

	"external/imgui/imgui.cpp"
	"external/imgui/imgui_demo.cpp"
	"external/imgui/imgui_draw.cpp"
	"external/imgui/imgui_tables.cpp"
	"external/imgui/imgui_widgets.cpp"
	"external/imgui/backends/imgui_impl_opengl3.cpp"
	"external/imgui/backends/imgui_impl_glfw.cpp"
	"external/imgui/backends/imgui_impl_opengl3.h"
	"external/imgui/backends/imgui_impl_glfw.h"

	"src/engine/frame.cpp" 
	"src/engine/frame.h" 
	"src/engine/shader.cpp" 
	"src/engine/shader.h"
	"src/engine/texture.cpp"
	"src/engine/texture.h" 
	"src/engine/camera.cpp" 
	"src/engine/camera.h"
)

target_include_directories(
	"vn_engine"
	PUBLIC
	"src/core"
	"src/engine"
	"src/runtime"
	"src/savefile"
	"src/utility"
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

project ("vn_runtime")


project ("local_test")

add_executable (
	"local_test"
	"local_test/local_test.cpp"
)

target_include_directories(
	"local_test"
	PUBLIC
	"local_test"
)