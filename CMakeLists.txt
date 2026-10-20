cmake_minimum_required(VERSION 3.20)

project(OpenGLEngine
    VERSION 1.0
    DESCRIPTION "OpenGL C++ Game Engine"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Download GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# Build GLAD
add_library(glad STATIC
    external/glad/src/gl.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

# Our application
add_executable(OpenGLEngine
    src/main.cpp
)

target_link_libraries(OpenGLEngine PRIVATE
    glfw
    glad
)

# macOS OpenGL framework
if(APPLE)
    find_library(OpenGL_FRAMEWORK OpenGL REQUIRED)
    target_link_libraries(OpenGLEngine PRIVATE
        ${OpenGL_FRAMEWORK}
    )
endif()