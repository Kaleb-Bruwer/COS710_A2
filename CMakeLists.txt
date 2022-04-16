cmake_minimum_required (VERSION 3.0)
project (Assignment2)

#C++ standard
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")

find_package(OpenGL REQUIRED)

add_subdirectory (external)

include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
)

add_Executable(main
    src/main.cpp
    src/CompShader.cpp
    src/DataLoader.cpp
    src/Helpers.cpp
    src/Tree.cpp
)

target_link_libraries(main ${OPENGL_LIBRARY} glfw GLEW_1130)
