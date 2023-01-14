# CMakeList.txt : CMake project for AStar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("AStar")

# Add source to this project's executable.
add_executable (AStar "main.cpp"  "inc/Grid.h" "inc/Grid.cpp"  "inc/Node.hpp"    "inc/Path.h" "inc/Path.cpp")
# add_executable(_Test "test.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AStar PROPERTY CXX_STANDARD 20)
endif()

# Link SFML
target_link_libraries(AStar ${CMAKE_SOURCE_DIR}/lib/sfml-graphics-d.lib)
#target_link_libraries(LearnSFML ${CMAKE_SOURCE_DIR}/lib/sfml-audio.lib)
target_link_libraries(AStar ${CMAKE_SOURCE_DIR}/lib/sfml-network-d.lib)
target_link_libraries(AStar ${CMAKE_SOURCE_DIR}/lib/sfml-window-d.lib)
target_link_libraries(AStar ${CMAKE_SOURCE_DIR}/lib/sfml-system-d.lib)

include_directories(${CMAKE_SOURCE_DIR}/inc)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
endif () 

