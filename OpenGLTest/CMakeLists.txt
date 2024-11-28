cmake_minimum_required(VERSION 3.20)

project(OpenGLTest_build)

set(CMAKE_CXX_STANDARD 17)
# set C++ version

add_library(glad src/glad.c)
# Find OpenGL and GLFW
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Create the executable
add_executable(OpenGLTest)

target_include_directories(OpenGLTest PRIVATE include)
target_include_directories(glad PRIVATE include)
# Include directories
target_sources(OpenGLTest PRIVATE ${CMAKE_SOURCE_DIR}/src/main.cpp ${CMAKE_SOURCE_DIR}/src/MyShader.cpp)

# Link libraries
target_link_libraries(OpenGLTest glad OpenGL::GL glfw)

include(GoogleTest)
enable_testing()
add_executable(MyShaderTests)
target_include_directories(MyShaderTests PRIVATE include)

target_sources(MyShaderTests PRIVATE ${CMAKE_SOURCE_DIR}/tests/MyShaderTests.cpp
                                     ${CMAKE_SOURCE_DIR}/src/MyShader.cpp
                                     ${CMAKE_SOURCE_DIR}/include/MyShader.h)
target_link_libraries(MyShaderTests GTest::gtest GTest::gtest_main glad OpenGL::GL glfw)

gtest_discover_tests(MyShaderTests)
