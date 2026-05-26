cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)

project(GameEngine VERSION 1.0)
set(GameEngine VERSION 1.0)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include(FetchContent)
FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.5
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(assimp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE GAME_ENGINE_SOURCES "src/*.h" "src/*.hpp" "src/*.cpp")

add_executable(GameEngine ${GAME_ENGINE_SOURCES})

target_link_libraries(GameEngine PRIVATE OpenGL)
target_link_libraries(GameEngine PRIVATE GLEW)
target_link_libraries(GameEngine PRIVATE glfw)
target_link_libraries(GameEngine PRIVATE assimp)
