cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
project(GameEngine VERSION 1.0)
set(GameEngine VERSION 1.0)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE GAME_ENGINE_SOURCES "src/*.hpp" "src/*.cpp")

add_executable(GameEngine ${GAME_ENGINE_SOURCES})

target_link_libraries(GameEngine OpenGL)
target_link_libraries(GameEngine GLEW)
target_link_libraries(GameEngine glfw)
