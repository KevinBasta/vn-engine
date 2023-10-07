# CMakeList.txt : CMake project for visual novel engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("visual novel engine")

# Add source to this project's executable.
add_executable (CMakeTarget "vn_engine.cpp" "vn_engine.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
