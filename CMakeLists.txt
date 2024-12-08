cmake_minimum_required (VERSION 3.11)

project ("testopengl")

# Get glfw library from online
include(FetchContent)

FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_MakeAvailable(glfw)

# Get glad library
add_subdirectory(glad)

add_executable("opengl-triangle"
    main.cpp
)
target_link_libraries("opengl-triangle" PRIVATE glfw GLAD)
