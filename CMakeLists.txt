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

FetchContent_Declare(imgui
    GIT_REPOSITORY git@github.com:ocornut/imgui.git
    GIT_TAG v1.92.9
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(imgui)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE GAME_ENGINE_SOURCES "src/main.cpp"
    "src/**/*.h" "src/**/*.hpp" "src/**/*.cpp")


add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

add_executable(GameEngine ${GAME_ENGINE_SOURCES})

target_link_libraries(GameEngine 
    PRIVATE OpenGL GLEW glfw assimp imgui)
