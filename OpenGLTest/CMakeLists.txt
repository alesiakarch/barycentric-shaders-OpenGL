cmake_minimum_required(VERSION 3.10)

project(OpenGLTest_build)

set(CMAKE_CXX_STANDARD 17)
# set C++ version

include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(glad src/glad.c)
# Find OpenGL and GLFW
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Create the executable
add_executable(OpenGLTest)

# Include directories
target_sources(OpenGLTest PRIVATE ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_include_directories(glad PRIVATE include)



# Link libraries
target_link_libraries(OpenGLTest glad OpenGL::GL glfw)
