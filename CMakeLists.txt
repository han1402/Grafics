#####################################
cmake_minimum_required(VERSION 3.0)
project(prog LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Release)
cmake_policy(SET CMP0072 NEW)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "-Werror -Wall -g")
##################################
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
include_directories(include ${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
file(GLOB SOURCES ./*.cpp)
##############################
add_executable(prog ${SOURCES})
target_link_libraries(prog ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES})
####################################
