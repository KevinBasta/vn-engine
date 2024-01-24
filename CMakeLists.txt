# CMakeList.txt : CMake project for visual_novel_engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("visual_novel_engine")

# Add source to this project's executable.
add_executable (
	visual_novel_engine
	"vn_engine.cpp"
)

target_sources(
	visual_novel_engine
	PRIVATE 
	"src/Json/json_lexer.cpp"
	"src/Json/json_token.cpp"
	"src/Json/json_error.cpp"
	"src/Json/json_expr.cpp"
	"src/Core/character.cpp"
	"src/Core/relation.cpp"
	"src/Core/graph.cpp"
	"src/Core/node.cpp"
	"src/Util/id.cpp"
	"src/Util/timer.cpp"
)

target_include_directories(
	visual_novel_engine
	BEFORE
	PUBLIC
	"src/Json"
)

target_include_directories(
	visual_novel_engine
	BEFORE
	PUBLIC
	"src/Core"
)

target_include_directories(
	visual_novel_engine
	BEFORE
	PUBLIC
	"src/Util"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET visual_novel_engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
