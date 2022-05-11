cmake_minimum_required (VERSION 3.0)
project (Assignment2)

#C++ standard and optimizers
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")


include_directories(
    src/
)

add_Executable(main
    src/main.cpp
    src/DataLoader.cpp
    src/Helpers.cpp
	src/Population/Population.cpp
	src/Population/TreeGen.cpp
    src/Population/Node.cpp
	src/Population/Tree.cpp
    src/Population/GPUNode.cpp
    src/Manager.cpp
    src/cpuExec.cpp
	src/Fitness.cpp
	src/Logger.cpp
)
