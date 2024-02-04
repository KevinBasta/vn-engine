# CMakeList.txt : CMake project for visual_novel_engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("visual_novel_engine")

# Add source to this project's executable.
add_executable (
	visual_novel_engine
	
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
	"src/Runtime/runtime_characters.h"
	"src/Runtime/runtime_relations.cpp"
	"src/Runtime/runtime_relations.h"
	"src/Runtime/runtime_chapters.cpp" 
	"src/Runtime/runtime_chapters.h" 
	
	"src/Util/timer.cpp"
)

target_include_directories(
	visual_novel_engine
	BEFORE
	PUBLIC
	"src/Core"
	"src/Engine"
	"src/Runtime"
	"src/Json"
	"src/Util"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET visual_novel_engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
