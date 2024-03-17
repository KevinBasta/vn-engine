# CMakeList.txt : CMake project for visual_novel_engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project("vn_engine")
set(CMAKE_CXX_STANDARD 20)



add_executable (
	"vn_engine"
	
	"vn_engine.cpp"

	"src/Json/json_lexer.cpp"
	"src/Json/json_token.cpp"
	"src/Json/json_error.cpp"
	"src/Json/json_expr.cpp"

	"src/Core/chapter.cpp"
	"src/Core/character.cpp"
	"src/Core/relation_types.cpp" 
	"src/Core/relation.cpp"
	"src/Core/graph.cpp"
	"src/Core/node.cpp"
	"src/Core/node_children.cpp" 
	"src/Core/id.cpp"
	
	"src/Runtime/runtime_characters.cpp"
	"src/Runtime/runtime_relations.cpp"
	"src/Runtime/runtime_chapters.cpp" 
	
	"src/Util/timer.cpp"

	"external/imgui/imgui.cpp"
	"external/imgui/imgui_demo.cpp"
	"external/imgui/imgui_draw.cpp"
	"external/imgui/imgui_tables.cpp"
	"external/imgui/imgui_widgets.cpp"
	"external/imgui/backends/imgui_impl_opengl3.cpp"
	"external/imgui/backends/imgui_impl_glfw.cpp"
)

target_include_directories(
	"vn_engine"
	PUBLIC
	"src/Core"
	"src/Engine"
	"src/Runtime"
	"src/Json"
	"src/Util"
	"external/imgui"
	"external/imgui/backends/imgui_impl_opengl3.h"
	"external/imgui/backends/imgui_impl_glfw.h"
)

add_subdirectory("external/glad")
add_subdirectory("external/glfw")
add_subdirectory("external/glm")

#file(GLOB_RECURSE opengl_inc_files "${opengl_inc}/*.h" "${opengl_inc}/*/*.h" "${opengl_inc}/*.hpp" "${opengl_inc}/*/*.hpp")
#file(GLOB_RECURSE opengl_src_files "${opengl_src}/*.c" "${opengl_src}/*.cpp")
#file(GLOB_RECURSE opengl_lib_files "${opengl_lib}/*.dll" "${opengl_lib}/*.lib")
#target_include_directories("vn_engine" PUBLIC "${opengl_inc_files}")
#target_sources("vn_engine" PRIVATE "${opengl_src_files}")
target_link_libraries("vn_engine" PRIVATE glad glfw glm)


project ("vn_runtime")


#find_package(OpenGL REQUIRED)
#include_directories("C:/OpenGL/inc/glad/")
#include_directories("C:/OpenGL/inc/KHR/")
#include_directories("C:/OpenGL/inc/GLFW/")
#include_directories("C:/OpenGL/inc/GLM/")

# Add source to this project's executable.



#target_link_libraries(visual_novel_engine ${OPENGL_LIBRARIES})


# TODO: Add tests and install targets if needed.
