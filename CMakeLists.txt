cmake_minimum_required(VERSION 3.26)
project(BIM)

set(CMAKE_CXX_STANDARD 17)

# Include GLFW headers
include_directories("${CMAKE_SOURCE_DIR}/include/")

# Your source files
add_executable(BIM srcs/main.cpp srcs/Input.cpp)

# Link against GLFW static library
target_link_libraries(BIM PRIVATE "${CMAKE_SOURCE_DIR}/lib/libglew32.a")
target_link_libraries(BIM PRIVATE "${CMAKE_SOURCE_DIR}/lib/libglfw3.a")

# Link against OpenGL32 library
target_link_libraries(BIM PRIVATE opengl32)

